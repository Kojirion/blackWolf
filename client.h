#ifndef CLIENT_H
#define CLIENT_H
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>
#include "common/position.h"
#include <boost/signals2.hpp>
#include "blackWolf.h"

class client
{
public:
    client();

    void connect();
    void update();
    //bool positionReady() const;
    position getPosition();

    boost::signals2::signal<void (int, int, int, int, int, int, Piece)> positionReady;
    boost::signals2::signal<void (Color, int, std::string, std::string)> startGame;
    boost::signals2::signal<void (Color)> gameEnd;
    boost::signals2::signal<void (std::string)> textReady;

    void makeMove(int row1, int col1, int row2, int col2, Piece promotionChoice = Piece::None);

    void toClient(std::string toWrite);

private:
    std::string nickname;

    boost::asio::streambuf data;
    boost::asio::io_service io_service;
    boost::asio::ip::tcp::socket socket;

    bool ready; //is a position ready?

    void handleData(boost::system::error_code ec);    

    int stringToCol(const std::string stringedCol) const;
    std::string moveString(const int row1, const int col1, const int row2, const int col2, Piece promotionChoice) const;
    std::string colToString(const int col) const;
    std::string pieceToSymbol(Piece piece) const;
    Piece symbolToPiece(std::string symbol) const;


};

#endif // CLIENT_H
