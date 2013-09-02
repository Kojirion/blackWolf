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

}

void client::update()
{
    if (!io_service.poll()) io_service.reset();
}


void client::handleData(boost::system::error_code ec)
{
    if (!ec)
    {
        std::string str((std::istreambuf_iterator<char>(&data)),
                       std::istreambuf_iterator<char>());

        boost::erase_all(str,"\n\r");

        std::vector<std::string> tokens;

        boost::split(tokens, str, boost::is_any_of(" "));

        if (!tokens.empty())
        {
            if (tokens[0] == "<12>")
            {
                int toPass[8][8];
                for (int i=0; i<8; ++i)
                {
                    for (int j=0; j<8; ++j)
                    {
                        toPass[i][j] = charToInt(tokens[8-i][j]);
                    }
                }
                position serverPos(toPass);
            }
        }

        boost::asio::async_read_until(socket, data, "\n\r",
                boost::bind(&client::handleData, this, _1));

    }
}

int client::charToInt(char x) const
{
    if (x == '-') return 0;
    else if (x == 'r') return -1;
    else if (x == 'n') return -3;
    else if (x == 'b') return -2;
    else if (x == 'q') return -4;
    else if (x == 'p') return -5;
    else if (x == 'k') return -6;
    else if (x == 'R') return 1;
    else if (x == 'N') return 3;
    else if (x == 'B') return 2;
    else if (x == 'Q') return 4;
    else if (x == 'P') return 5;
    else if (x == 'K') return 6;
}
