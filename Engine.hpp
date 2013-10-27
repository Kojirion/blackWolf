#ifndef CHESSENGINE_H
#define CHESSENGINE_H
#include <boost/process.hpp>
#include <iostream>
#include "BlackWolf.hpp"
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "Entity.hpp"

class Engine : private Entity
{

public:

    Engine();
    ~Engine();

    void makeMove(const Move& move, const int pieceChoice = 0);

    void newGame();

    Move getMove();

    bool load();

    void unLoad();

    void update();

    int getPromotionChoice() const;


private:
    boost::process::pipe engineOut;
    boost::process::pipe engineIn;

    boost::iostreams::file_descriptor_sink engineOutSink;
    boost::iostreams::file_descriptor_source engineOutSource;

    boost::iostreams::file_descriptor_sink engineInSink;
    boost::iostreams::file_descriptor_source engineInSource;

    boost::iostreams::stream<boost::iostreams::file_descriptor_source> engineOutStream;
    boost::iostreams::stream<boost::iostreams::file_descriptor_sink> engineInStream;

    void waitForOk();

    std::string moveString(const Move& move) const;

    std::string colToString(const int col) const;
    int stringToCol(const std::string stringedCol) const;

    Move stringToTuple(const std::string theString);

    std::string moveList;

    void toEngine(const std::string toPut);
    std::string fromEngine();


    int symbolToInt(std::string symbol) const;
    std::string intToSymbol(const int which) const;
    int promotedChoice;

    bool loaded;

    void handleData(boost::system::error_code ec);

    boost::asio::streambuf data;

    boost::asio::io_service io_service;

#if defined(BOOST_WINDOWS_API)
    typedef boost::asio::windows::stream_handle pipe_end;
#elif defined(BOOST_POSIX_API)
    typedef boost::asio::posix::stream_descriptor pipe_end;
#endif

    pipe_end pend;

};

#endif // CHESSENGINE_H
