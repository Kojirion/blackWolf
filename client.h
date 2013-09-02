#ifndef CLIENT_H
#define CLIENT_H
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>
#include "common/position.h"

class client
{
public:
    client();

    void connect();
    void update();
    bool positionReady() const;
    position getPosition();

private:
    boost::asio::streambuf data;
    boost::asio::io_service io_service;
    boost::asio::ip::tcp::socket socket;

    bool ready; //is a position ready?

    void handleData(boost::system::error_code ec);
};

#endif // CLIENT_H
