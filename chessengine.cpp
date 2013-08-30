#include "chessengine.h"
#include "options.h"
#include <boost/ptr_container/ptr_vector.hpp>


void chessEngine::waitForOk()
{
    toEngine("isready");

    while (fromEngine()!="readyok"){
    }
}

std::string chessEngine::moveString(const int row1, const int col1, const int row2, const int col2) const
{
   return (colToString(col1) + std::to_string(row1+1) + colToString(col2) + std::to_string(row2+1));
}

std::string chessEngine::colToString(const int col) const
{
    switch (col) {
    case 0:
        return "a";
    case 1:
        return "b";
    case 2:
        return "c";
    case 3:
        return "d";
    case 4:
        return "e";
    case 5:
        return "f";
    case 6:
        return "g";
    case 7:
        return "h";
    }
}

int chessEngine::stringToCol(const std::string stringedCol) const
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

chessEngine::move chessEngine::stringToTuple(const std::string theString)
{
    const int row1 = std::stoi(theString.substr(1,1)) - 1;
    const int col1 = stringToCol(theString.substr(0,1));
    const int row2 = std::stoi(theString.substr(3,1)) - 1;
    const int col2 = stringToCol(theString.substr(2,1));

    if (theString.size()==5) promotedChoice = symbolToInt(theString.substr(4,1));

    return std::make_tuple(row1,col1,row2,col2);
}

void chessEngine::toEngine(const std::string toPut)
{
    engineInStream << toPut << std::endl;
}

std::string chessEngine::fromEngine()
{
    std::string toReturn;
    engineOutStream >> toReturn;
    return toReturn;
}

int chessEngine::symbolToInt(std::string symbol) const
{
    if (symbol=="q") return 4;
    if (symbol=="b") return 2;
    if (symbol=="n") return 3;
    if (symbol=="r") return 1;
}

std::string chessEngine::intToSymbol(const int which) const
{
    if (which == 4) return "q";
    if (which == 2) return "b";
    if (which == 3) return "n";
    if (which == 1) return "r";
}



chessEngine::chessEngine():
    engineOut(boost::process::create_pipe()),
    engineIn(boost::process::create_pipe()),
    engineOutSink(engineOut.sink, boost::iostreams::close_handle),
    engineOutSource(engineOut.source, boost::iostreams::close_handle),
    engineInSink(engineIn.sink, boost::iostreams::close_handle),
    engineInSource(engineIn.source, boost::iostreams::close_handle),
    engineOutStream(engineOutSource),
    engineInStream(engineInSink)
{

}

void chessEngine::makeMove(const int row1, const int col1, const int row2, const int col2, const int pieceChoice)
{
    std::string toAdd = moveString(row1,col1,row2,col2);
    if (pieceChoice!=0) toAdd += intToSymbol(pieceChoice);
    moveList += toAdd + " ";
}

void chessEngine::newGame()
{
    toEngine("ucinewgame");
    moveList.clear();
    waitForOk();
}

chessEngine::move chessEngine::getMove()
{
    toEngine("position startpos moves " + moveList);
    toEngine("go depth 8");

    while(true){
        if (fromEngine()=="bestmove")
        {
            return stringToTuple(fromEngine());
        }
    }
}

bool chessEngine::load()
{
    boost::process::execute(boost::process::initializers::run_exe("./stockfish"),
                            boost::process::initializers::bind_stdout(engineOutSink),
                            boost::process::initializers::bind_stdin(engineInSource));

    toEngine("uci");

    //boost::ptr_vector<option> options;

    while (fromEngine()!="uciok")
    {
        if (fromEngine()=="option")
        {
            std::string name = fromEngine();
        }
    }

    waitForOk();

    return true;
}

void chessEngine::unLoad()
{
    toEngine("quit");
}

int chessEngine::getPromotionChoice() const
{
    return promotedChoice;
}
