#ifndef COMPLETEMOVE_H
#define COMPLETEMOVE_H
#include "boardmove.h"

class completeMove : private boardMove
{
public:
    completeMove(position &thePosition, const int theRow1, const int theCol1, const int theRow2, const int theCol2);

    int sideInCheck(); //checks which side is in check in the new position
};

#endif // COMPLETEMOVE_H
