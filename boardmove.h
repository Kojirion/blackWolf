#ifndef BOARDMOVE_H
#define BOARDMOVE_H
#include "position.h"

class boardMove
{
private:
    position &board;

    int row1;
    int col1;
    int row2;
    int col2;

    int pieceCode;

    bool isOccupied() const;
    bool isKnightLegal() const;
    bool isBishopLegal() const;
    bool isRookLegal() const;
    bool isQueenLegal() const;
    bool isKingLegal() const;
    bool isPawnLegal() const;



public:
    boardMove(position &thePosition, const int theRow1, const int theCol1, const int theRow2, const int theCol2);

    position newBoard;
};

#endif // BOARDMOVE_H
