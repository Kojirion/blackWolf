#include "Client.hpp"
#include "parsers/GameStateParser.hpp"
#include "parsers/SessionStartParser.hpp"
#include "parsers/GameStartParser.hpp"
#include "parsers/GameEndParser.hpp"
#include <boost/fusion/container/vector.hpp>
#include "Timeseal.hpp"
#include "generators/MoveStringGrammar.hpp"
#include <boost/spirit/include/qi.hpp>
#include <boost/variant.hpp>

Client::Client():
    outputStream(&output),
    socket(io_service)
{
}

void Client::connect()
{
    outputStream.flush();
    output.consume(output.size());

    boost::asio::ip::tcp::resolver resolver(io_service);
    boost::asio::ip::tcp::resolver::query query("freechess.org","5000");
    boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
    boost::asio::ip::tcp::resolver::iterator end;
    
    boost::system::error_code error = boost::asio::error::host_not_found;
    
    while(error && endpoint_iterator != end)
    {
        socket.close();
        socket.connect(*endpoint_iterator++, error);
    }
    
    if (error)
    {
        textReady("Failed to connect.");
        std::cerr << "Failed to connect.\n";
        return;
    }
    
    std::string hello = "TIMESTAMP|openseal|Running on an operating system|";
    toClient(hello);
    
    boost::asio::async_read_until(socket, data, "\n\r",
                                  boost::bind(&Client::handleData, this, _1));
}

void Client::update()
{
    if (!socket.is_open())
        return;

    boost::asio::write(socket, output);
    
    if (!io_service.poll())
        io_service.reset();
}

struct TriggerMessage
        : public boost::static_visitor<>
{
    TriggerMessage(MessageSystem& messages):messages(messages){}

    void operator()(Message& message) const
    {
        messages.triggerEvent(message);
    }

    MessageSystem& messages;
};

void Client::handleData(boost::system::error_code ec)
{
    if (!ec)
    {
        std::istream is(&data);
        std::string str;
        while(std::getline(is, str, '\r')){
            if (str == "[G]\n")
                toClient("\x2""9");

            
            using boost::spirit::qi::parse;
            
            static GameEndParser gameEndParser;
            static GameStartParser gameStartParser;
            static SessionStartParser sessionStartParser;
            static GameStateParser gameStateParser;

            static auto onGameStart = [this](NewGameMessage& gameStart){
                gameStart.user = gameStart.p1 == nickname ? Color::White : Color::Black;
            };

            static boost::spirit::qi::rule<Iterator, NewGameMessage()> gameStartGrammar;
            gameStartGrammar %= gameStartParser[onGameStart];

            using ParsedMessage = boost::variant<GameStateMessage, NewGameMessage, EndGameMessage>;

            static boost::spirit::qi::rule<Iterator, ParsedMessage()> lineGrammar = gameStateParser | gameStartGrammar | gameEndParser;

            static auto onMeaningfulLine = [this](ParsedMessage& message){
                boost::apply_visitor(TriggerMessage(messages), message);
            };

            static boost::spirit::qi::rule<Iterator, ParsedMessage()> lineGrammarMessage = lineGrammar[onMeaningfulLine];

            if (!parse(str.begin(), str.end(), lineGrammarMessage)){
                if(parse(str.begin(), str.end(), sessionStartParser, nickname)){
                    toClient("set style 12");
                }else{
                    //not processed, so let's print it
                    textReady(str);
                }
            }
        }
        boost::asio::async_read_until(socket, data, "\n\r",
                                      boost::bind(&Client::handleData, this, _1));
    }
}

void Client::toClient(std::string toWrite)
{
    crypt(toWrite);
    outputStream << toWrite;
}

void Client::makeMove(const Move &move)
{
    using Iterator = std::back_insert_iterator<std::string>;

    static const MoveStringGrammar<Iterator> moveStringGrammar;

    std::string moveString;
    boost::spirit::karma::generate(Iterator(moveString), moveStringGrammar, move);

    toClient(std::move(moveString));
}
