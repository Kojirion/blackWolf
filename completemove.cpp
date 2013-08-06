#include "completemove.h"

bool completeMove::isCheckSafe() const
{
    const int checkedSide = sideInCheck();
    if (checkedSide == 2) return false;
    if (checkedSide*board.turnColor>0) return false;
    return true;
}

completeMove::completeMove(position &thePosition, const int theRow1, const int theCol1, const int theRow2, const int theCol2):
    boardMove(thePosition, theRow1, theCol1, theRow2, theCol2)
{
}

int completeMove::sideInCheck() const
{
    //returns which side is in check, 2 for both
    int whichSide = 0;

    int whiteKingRow, whiteKingCol, blackKingRow, blackKingCol;

    //find where the kings are
    for (int i=0; i<8; ++i){
        for (int j=0; j<8; ++j){
            const int pieceId = newBoard[i][j];
            if (pieceId==6){
                whiteKingRow = i;
                whiteKingCol = j;
            }else if (pieceId==-6){
                blackKingRow = i;
                blackKingCol = j;
            }
        }
    }

    for (int i=0; i<8; ++i){
        for (int j=0; j<8; ++j){
            const int pieceId = newBoard[i][j];
            if (pieceId>0){
                boardMove toCheck(newBoard,i,j,blackKingRow,blackKingCol);
                if (toCheck.isLegal()){
                    if (whichSide==1) whichSide = 2;
                    else whichSide = -1;
                }
            }else if (pieceId<0){
                boardMove toCheck(newBoard,i,j,whiteKingRow,whiteKingCol);
                if (toCheck.isLegal()){
                    if (whichSide==-1) whichSide = 2;
                    else whichSide = 1;
                }
            }
        }
    }

    return whichSide;
}

bool completeMove::isLegal() const
{
    if ((row1==row2)&&(col1==col2)) return false; //start and end are the same

    if (!boardMove::isLegal()) return false;

    return isCheckSafe();
}

position completeMove::getNewBoard() const
{
    return newBoard;
}
