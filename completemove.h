#ifndef COMPLETEMOVE_H
#define COMPLETEMOVE_H
#include "boardmove.h"

class completeMove : private boardMove
{
private:
    bool isCheckSafe() const;
public:
    completeMove(position &thePosition, const int theRow1, const int theCol1, const int theRow2, const int theCol2);

    bool isLegal() const;

    position getNewBoard() const;


};

#endif // COMPLETEMOVE_H
