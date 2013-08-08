#include "chessengine.h"

void chessEngine::waitForOk()
{
    process << "isready" << std::endl;

    std::string str;
    while (str!="readyok"){
        process >> str;
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

chessEngine::move chessEngine::stringToTuple(const std::string theString) const
{
    const int row1 = std::stoi(theString.substr(1,1));
    const int col1 = stringToCol(theString.substr(0,1));
    const int row2 = std::stoi(theString.substr(3,1));
    const int col2 = stringToCol(theString.substr(2,1));

    return std::make_tuple(row1,col1,row2,col2);
}


chessEngine::chessEngine():
    process("./stockfish")
{

}

void chessEngine::makeMove(const int row1, const int col1, const int row2, const int col2)
{
    std::string toAdd = moveString(row1,col1,row2,col2);
    moveList += toAdd + " ";
}

void chessEngine::newGame()
{
    process << "ucinewgame" << std::endl;
    //process << redi::pstreambuf::kill() << std::endl;
    waitForOk();
}

chessEngine::move chessEngine::getMove()
{
    process << ("position startpos moves " + moveList) << std::endl;
    process << "go depth 8" << std::endl;

    std::string str;

    while (process >> str){
        if (str=="bestmove"){
            process >> str;
            std::cout << "The best move is" << str << std::endl;
        }
    }
}

bool chessEngine::load()
{
    process << "uci" << std::endl;

    std::string str;
    while (str!="uciok"){
        process >> str;
    }

    waitForOk();

    return true;
}

void chessEngine::unLoad()
{
    process << "quit" << std::endl;
    //process.rdbuf->kill();
    while (!process.rdbuf()->exited()){}
    process.rdbuf()->status(); //throw away the exit status
}
