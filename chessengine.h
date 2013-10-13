#ifndef CHESSENGINE_H
#define CHESSENGINE_H
#include <boost/process.hpp>
#include <iostream>
#include "blackWolf.h"
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/file_descriptor.hpp>

class chessEngine
{

public:

    chessEngine();

    void makeMove(const Move& move, const int pieceChoice = 0);

    void newGame();

    Move getMove();

    bool load();

    void unLoad();

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

};

#endif // CHESSENGINE_H
