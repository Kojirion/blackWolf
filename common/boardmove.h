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

    Move m_move;

    position newBoard;

    Unit m_piece;


    bool startEndSame() const;
    bool isOccupied() const;
    bool isKnightLegal() const;
    bool isBishopLegal() const;
    bool isRookLegal() const;
    bool isQueenLegal() const;
    bool isKingLegal() const;
    bool isPawnLegal() const;

    bool isObstructed(Unit piece) const; //returns true if square occupied



public:
    boardMove(const position &thePosition, const Move &move);


    bool isLegal() const;
};

#endif // BOARDMOVE_H
