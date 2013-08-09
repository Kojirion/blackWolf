#ifndef COMPLETEMOVE_H
#define COMPLETEMOVE_H
#include "boardmove.h"

class completeMove : private boardMove
{
private:
    bool isCheckSafe() const;
    bool handleCastle() const;

    bool inCheck(const position &givenPos, const int side) const; //is given side in check?

    bool hasLegalMoves() const; //checks if the new side will have legal moves in new position


public:
    completeMove(const position &thePosition, const int theRow1, const int theCol1, const int theRow2, const int theCol2);

    bool isLegal() const;

    bool isCheckmate() const;
    bool isStalemate() const;

    int getRow1() const;
    int getCol1() const;
    int getRow2() const;
    int getCol2() const;

    position getNewBoard() const;


};

#endif // COMPLETEMOVE_H
