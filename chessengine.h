#ifndef CHESSENGINE_H
#define CHESSENGINE_H
#include <boost/process.hpp>
#include <iostream>
#include <tuple>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/file_descriptor.hpp>

class chessEngine
{

public:
    typedef std::tuple<int,int,int,int> move;

    chessEngine();

    void makeMove(const int row1, const int col1, const int row2, const int col2, const int pieceChoice = 0);

    void newGame();

    move getMove();

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

    std::string moveString(const int row1, const int col1, const int row2, const int col2) const;

    std::string colToString(const int col) const;
    int stringToCol(const std::string stringedCol) const;

    move stringToTuple(const std::string theString);

    std::string moveList;

    void toEngine(const std::string toPut);
    std::string fromEngine();


    int symbolToInt(std::string symbol) const;
    std::string intToSymbol(const int which) const;
    int promotedChoice;

};

#endif // CHESSENGINE_H
