#include "Engine.hpp"
#include <boost/ptr_container/ptr_vector.hpp>


void Engine::waitForOk()
{
    toEngine("isready");

    while (fromEngine()!="readyok"){
    }
}

std::string Engine::moveString(const Move &move) const
{
   return (colToString(move.square_1.col) + std::to_string(move.square_1.row+1)
           + colToString(move.square_2.col) + std::to_string(move.square_2.row+1));
}

std::string Engine::colToString(const int col) const
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
    default:
        return "-"; //appease compiler
    }
}

int Engine::stringToCol(const std::string stringedCol) const
{
    if (stringedCol=="a") return 0;
    else if (stringedCol=="b") return 1;
    else if (stringedCol=="c") return 2;
    else if (stringedCol=="d") return 3;
    else if (stringedCol=="e") return 4;
    else if (stringedCol=="f") return 5;
    else if (stringedCol=="g") return 6;
    else if (stringedCol=="h") return 7;
    return -1; //appease compiler
}

Move Engine::stringToTuple(const std::string theString)
{
    const int row1 = std::stoi(theString.substr(1,1)) - 1;
    const int col1 = stringToCol(theString.substr(0,1));
    const int row2 = std::stoi(theString.substr(3,1)) - 1;
    const int col2 = stringToCol(theString.substr(2,1));

    if (theString.size()==5) promotedChoice = symbolToInt(theString.substr(4,1));

    return {{row1,col1},{row2,col2}};
}

void Engine::toEngine(const std::string toPut)
{
    engineInStream << toPut << std::endl;
}

std::string Engine::fromEngine()
{
    std::string toReturn;
    engineOutStream >> toReturn;
    return toReturn;
}

int Engine::symbolToInt(std::string symbol) const
{
    if (symbol=="q") return 4;
    if (symbol=="b") return 2;
    if (symbol=="n") return 3;
    if (symbol=="r") return 1;
    return -1; //appease compiler
}

std::string Engine::intToSymbol(const int which) const
{
    if (which == 4) return "q";
    if (which == 2) return "b";
    if (which == 3) return "n";
    if (which == 1) return "r";
    return "-"; //appease compiler
}



Engine::Engine():
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

void Engine::makeMove(const Move &move, const int pieceChoice)
{
    std::string toAdd = moveString(move);
    if (pieceChoice!=0) toAdd += intToSymbol(pieceChoice);
    moveList += toAdd + " ";
}

void Engine::newGame()
{
    toEngine("ucinewgame");
    moveList.clear();
    waitForOk();
}

Move Engine::getMove()
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

bool Engine::load()
{
    boost::process::execute(boost::process::initializers::run_exe("./stockfish"),
                            boost::process::initializers::bind_stdout(engineOutSink),
                            boost::process::initializers::bind_stdin(engineInSource));

    toEngine("uci");

    //boost::ptr_vector<option> options;

    while (fromEngine()!="uciok")
    {
        /*if (fromEngine()=="option")
        {
            std::string name = fromEngine();
        }*/
    }

    waitForOk();

    return true;
}

void Engine::unLoad()
{
    toEngine("quit");
}

int Engine::getPromotionChoice() const
{
    return promotedChoice;
}
