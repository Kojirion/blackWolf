#ifndef COMPLETEMOVE_H
#define COMPLETEMOVE_H
#include "PseudoMove.hpp"

class CompleteMove : private PseudoMove
{
private:
    bool isCheckSafe() const;
    bool handleCastle() const;

    bool inCheck(const Position &givenPos, Color side) const;

    bool hasLegalMoves() const; //checks if the new side will have legal moves in new position

    Square findKing(const Position& position, Color color) const;

    bool isLegalColorblind() const;

    Castle getCastle() const;


public:
    CompleteMove(const Position &thePosition, const Move& move);

    bool isLegal() const;

    bool isCheckmate() const;
    bool isStalemate() const;

    const Move& getMove() const;

    Position getNewBoard() const;


};

#endif // COMPLETEMOVE_H
