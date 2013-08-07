#include "completemove.h"

bool completeMove::isCheckSafe() const
{
    int kingRow, kingCol;

    for (int i=0; i<8; ++i){
        bool found = false;
        for (int j=0; j<8; ++j){
            const int pieceId = newBoard[i][j];
            if (pieceId==6*board.turnColor){ //found our king
                kingRow = i;
                kingCol = j;
                break;
            }
        }
        if (found) break;
    }

    for (int i=0; i<8; ++i){
        for (int j=0; j<8; ++j){
            const int pieceId = newBoard[i][j];
            if (pieceId*board.turnColor<0){ //enemy piece
                boardMove toCheck(newBoard,i,j,kingRow,kingCol);
                if (toCheck.isLegal()) return false;
            }
        }
    }

    return true;
}

completeMove::completeMove(position &thePosition, const int theRow1, const int theCol1, const int theRow2, const int theCol2):
    boardMove(thePosition, theRow1, theCol1, theRow2, theCol2)
{
}

bool completeMove::isLegal() const
{
    if (!boardMove::isLegal()) return false;

    return isCheckSafe();
}

position completeMove::getNewBoard() const
{
    return newBoard;
}
