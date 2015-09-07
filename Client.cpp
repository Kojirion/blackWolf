#include "Client.hpp"
#include "parsers/GameStateParser.hpp"
#include "parsers/SessionStartParser.hpp"
#include "parsers/GameStartParser.hpp"
#include "parsers/GameEndParser.hpp"
#include <boost/fusion/container/vector.hpp>
#include "Timeseal.hpp"

Client::Client():
    outputStream(&output),
    socket(io_service)
{
}

void Client::connect()
{
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
    
    //std::string hello = "TIMESTAMP|openseal|Running on an operating system|";
    //toClient(hello);

    //    int n=crypt(hello);
    //    toClient(hello);
    
    boost::asio::async_read_until(socket, data, "\n\r",
                                  boost::bind(&Client::handleData, this, _1));

}

void Client::update()
{
    boost::asio::write(socket, output);
    
    if (!io_service.poll())
        io_service.reset();
}

void Client::handleData(boost::system::error_code ec)
{
    if (!ec)
    {
        std::istream is(&data);
        std::string str;
        while(std::getline(is, str, '\r')){
            
            using boost::spirit::qi::parse;
            
            static const GameEndParser gameEndParser;
            static const GameStartParser gameStartParser;
            static const SessionStartParser sessionStartParser;
            static const GameStateParser gameStateParser;
            GameStateMessage gameState;
            
            if (parse(str.begin(), str.end(), gameStateParser, gameState)){
                messages.triggerEvent(gameState);
            }else{
                NewGameMessage gameStart;
                
                if (parse(str.begin(), str.end(), gameStartParser, gameStart)){
                    if (gameStart.p1 == nickname)
                        gameStart.user = Color::White;
                    else
                        gameStart.user = Color::Black;
                    messages.triggerEvent(gameStart);
                }else{
                    EndGameMessage endGame;
                    if (parse(str.begin(), str.end(), gameEndParser, endGame)){
                        messages.triggerEvent(endGame);
                    }else{
                        if(parse(str.begin(), str.end(), sessionStartParser, nickname)){
                            toClient("set style 12");
                        }else{
                            //not processed, so let's print it
                            textReady(str);
                        }
                    }
                }
            }
        }
        
        boost::asio::async_read_until(socket, data, "\n\r",
                                      boost::bind(&Client::handleData, this, _1));
    }
}

void Client::toClient(const std::string& toWrite)
{
    outputStream << toWrite << "\r\n";
}

int Client::stringToCol(const std::string& stringedCol) const
{
    if (stringedCol=="a") return 0;
    else if (stringedCol=="b") return 1;
    else if (stringedCol=="c") return 2;
    else if (stringedCol=="d") return 3;
    else if (stringedCol=="e") return 4;
    else if (stringedCol=="f") return 5;
    else if (stringedCol=="g") return 6;
    else if (stringedCol=="h") return 7;
    return -1; //appease compiler
}

std::string Client::moveString(const Move &move, Piece promotionChoice) const
{
    std::string toReturn(colToString(move.square_1.col) + std::to_string(move.square_1.row+1)
                         + colToString(move.square_2.col) + std::to_string(move.square_2.row+1));
    if (promotionChoice != Piece::None) toReturn += "=" + pieceToSymbol(promotionChoice);
    return toReturn;
}

std::string Client::colToString(const int col) const
{
    switch (col) {
    case 0:
        return "a";
    case 1:
        return "b";
    case 2:
        return "c";
    case 3:
        return "d";
    case 4:
        return "e";
    case 5:
        return "f";
    case 6:
        return "g";
    case 7:
        return "h";
    default:
        return "-"; //appease compiler
    }
}

std::string Client::pieceToSymbol(Piece piece) const
{
    if (piece == Piece::Queen) return "Q";
    if (piece == Piece::Bishop) return "B";
    if (piece == Piece::Knight) return "N";
    if (piece == Piece::Rook) return "R";
    return "-"; //appease compiler
}

void Client::makeMove(const Move &move, Piece promotionChoice)
{
    toClient(moveString(move,promotionChoice));
}
