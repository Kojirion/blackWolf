#include "Client.hpp"

Client::Client():
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
        std::cerr << "Failed to conect." << std::endl;
        return;
    }

    boost::asio::async_read_until(socket, data, "\n\r",
                                  boost::bind(&Client::handleData, this, _1));
}

void Client::update()
{
    if (!io_service.poll()) io_service.reset();
}


void Client::handleData(boost::system::error_code ec)
{
    if (!ec)
    {
        std::istream is(&data);
        std::string str;
        std::getline(is, str);

        boost::erase_all(str,"\r");

        std::vector<std::string> tokens;


        boost::split(tokens, str, boost::is_any_of(" "));

        if (!tokens.empty())
        {
            if ((tokens[0] == "<12>")&&(tokens[27]!="none"))
            {
                int row1;
                int col1;
                int row2;
                int col2;

                if (tokens[27]=="o-o")
                {
                    if (tokens[9]=="W")
                    {
                        row1 = 7;
                        col1 = 4;
                        row2 = 7;
                        col2 = 6;
                    }else{
                        row1 = 0;
                        col1 = 4;
                        row2 = 0;
                        col2 = 6;
                    }
                }else if (tokens[27]=="o-o-o"){
                    if (tokens[9]=="W")
                    {
                        row1 = 7;
                        col1 = 4;
                        row2 = 7;
                        col2 = 2;
                    }else{
                        row1 = 0;
                        col1 = 4;
                        row2 = 0;
                        col2 = 2;
                    }

                }else{
                    row1 = std::stoi(tokens[27].substr(3,1)) - 1;
                    col1 = stringToCol(tokens[27].substr(2,1));
                    row2 = std::stoi(tokens[27].substr(6,1)) - 1;
                    col2 = stringToCol(tokens[27].substr(5,1));
                }

                int whiteTime = std::stoi(tokens[24]);
                int blackTime = std::stoi(tokens[25]);

                Piece promotionPiece;

                if (tokens[27].size()==9){ //means promotion
                    promotionPiece = symbolToPiece(tokens[27].substr(8,1));
                }else promotionPiece = Piece::None;


                positionReady({{row1,col1},{row2,col2}}, whiteTime, blackTime, promotionPiece);
            }else if (tokens[0] == "Creating:"){
                int time = 60*std::stoi(tokens[7]);
                if (tokens[1] == nickname) startGame(Color::White, time, tokens[1], tokens[3]);
                else startGame(Color::Black, time, tokens[1], tokens[3]);
            }else if (tokens[0] == "{Game"){
                std::string toCheck = tokens.back();
                if (toCheck == "1-0") gameEnd(Color::White);
                else if (toCheck == "0-1") gameEnd(Color::Black);
                else if (toCheck == "1/2-1/2") gameEnd(Color::Both);
            }else if ((tokens[0]=="****")&&(tokens[1]=="Starting")&&(tokens[2]=="FICS")){
                //succesfully logged in
                boost::erase_all(tokens[5],"(U)");
                boost::erase_all(tokens[5],"(R)");
                nickname = tokens[5];
                toClient("set style 12");
            }else if (tokens[0] != "fics%"){
                //not processed, so let's print it
                textReady(str);
            }
        }

        boost::asio::async_read_until(socket, data, "\n\r",
                                      boost::bind(&Client::handleData, this, _1));

    }
}

void Client::toClient(const std::string& toWrite)
{
    socket.write_some(boost::asio::buffer(toWrite + "\r\n"));
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

Piece Client::symbolToPiece(std::string symbol) const
{
    if (symbol == "Q") return Piece::Queen;
    if (symbol == "B") return Piece::Bishop;
    if (symbol == "N") return Piece::Bishop;
    if (symbol == "R") return Piece::Rook;
    return Piece::None; //appease compiler
}


void Client::makeMove(const Move &move, Piece promotionChoice)
{
    toClient(moveString(move,promotionChoice));
}
