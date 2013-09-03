#include "client.h"

client::client():
     socket(io_service)
{
}

void client::connect()
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

    toClient("Kojijay");
    toClient("");
    toClient("match Kojay");

    boost::asio::async_read_until(socket, data, "\n\r",
            boost::bind(&client::handleData, this, _1));
}

void client::update()
{
    if (!io_service.poll()) io_service.reset();
}


void client::handleData(boost::system::error_code ec)
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
            if (tokens[0] == "<12>")
            {
                const int row1 = std::stoi(tokens[27].substr(3,1)) - 1;
                const int col1 = stringToCol(tokens[27].substr(2,1));
                const int row2 = std::stoi(tokens[27].substr(6,1)) - 1;
                const int col2 = stringToCol(tokens[27].substr(5,1));

                positionReady(row1,col1,row2,col2);
            }else if (tokens[0] == "Creating:")
            {
                if (tokens[1] == "Kojijay") startGame(bw::White);
                else startGame(bw::Black);
            }
        }

        boost::asio::async_read_until(socket, data, "\n\r",
                boost::bind(&client::handleData, this, _1));

    }
}

void client::toClient(std::string toWrite)
{
    toWrite += "\r\n";
    socket.write_some(boost::asio::buffer(toWrite));
}

int client::stringToCol(const std::string stringedCol) const
{
    if (stringedCol=="a") return 0;
    else if (stringedCol=="b") return 1;
    else if (stringedCol=="c") return 2;
    else if (stringedCol=="d") return 3;
    else if (stringedCol=="e") return 4;
    else if (stringedCol=="f") return 5;
    else if (stringedCol=="g") return 6;
    else if (stringedCol=="h") return 7;
}
