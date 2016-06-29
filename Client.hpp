#pragma once
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>
#include "BlackWolf.hpp"
#include "messages/Entity.hpp"

class Client : private Messages::Registrant
{
public:
    Client();

    void connect();
    void update();

    void makeMove(const Move& move);

    void toClient(std::string toWrite);

private:
    std::string nickname;

    boost::asio::streambuf data, output;
    std::ostream outputStream;
    boost::asio::io_service io_service;
    boost::asio::ip::tcp::socket socket;

    void handleData(boost::system::error_code ec);
};
