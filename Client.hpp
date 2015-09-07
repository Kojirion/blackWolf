#pragma once
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/signals2.hpp>
#include "BlackWolf.hpp"
#include "Entity.hpp"

class Client : private Entity
{
public:
    Client();

    void connect();
    void update();

    boost::signals2::signal<void (std::string)> textReady;

    void makeMove(const Move& move, Piece promotionChoice = Piece::None);

    void toClient(const std::string &toWrite);

private:
    std::string nickname;

    boost::asio::streambuf data, output;
    std::ostream outputStream;
    boost::asio::io_service io_service;
    boost::asio::ip::tcp::socket socket;

    void handleData(boost::system::error_code ec);    

    int stringToCol(const std::string &stringedCol) const;
    std::string moveString(const Move& move, Piece promotionChoice) const;
    std::string colToString(const int col) const;
    std::string pieceToSymbol(Piece piece) const;
};
