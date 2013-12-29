#ifndef BOARDMOVE_H
#define BOARDMOVE_H
#include "Position.hpp"

class PseudoMove
{
protected:
    Position m_board;

    Move m_move;

    Position newBoard;

    Unit m_piece;


    bool startEndSame() const;
    bool isOccupied() const;
    bool isKnightLegal() const;
    bool isBishopLegal() const;
    bool isRookLegal() const;
    bool isQueenLegal() const;
    bool isKingLegal() const;
    bool isPawnLegal() const;

    bool isObstructed(const Unit& unit) const; //returns true if square occupied



public:
    PseudoMove(const Position &thePosition, const Move &move);


    bool isLegal() const;
};

#endif // BOARDMOVE_H
