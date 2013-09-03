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

    boost::signals2::signal<void (int, int, int, int)> positionReady;
    boost::signals2::signal<void (bw whoUser)> startGame;

private:
    boost::asio::streambuf data;
    boost::asio::io_service io_service;
    boost::asio::ip::tcp::socket socket;

    bool ready; //is a position ready?

    void handleData(boost::system::error_code ec);
    void toClient(std::string toWrite);

    int stringToCol(const std::string stringedCol) const;


};

#endif // CLIENT_H
