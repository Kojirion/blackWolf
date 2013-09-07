//Describes a move by
//The starting position
//Four ints to describe starting and end square
//The final position

#ifndef BOARDMOVE_H
#define BOARDMOVE_H
#include "position.h"

class boardMove
{
private:
    friend class completeMove;

    position board;    

    int row1;
    int col1;
    int row2;
    int col2;

    position newBoard;

    int pieceCode;


    bool startEndSame() const;
    bool isOccupied() const;
    bool isKnightLegal() const;
    bool isBishopLegal() const;
    bool isRookLegal() const;
    bool isQueenLegal() const;
    bool isKingLegal() const;
    bool isPawnLegal() const;

    bool isObstructed(const int pieceCode) const; //returns true if square occupied



public:
    boardMove(const position &thePosition, const int theRow1, const int theCol1, const int theRow2, const int theCol2);


    bool isLegal() const;
};

#endif // BOARDMOVE_H
