#include "completemove.h"
#include <boost/assert.hpp>

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

bool completeMove::handleCastle() const
{
    if (inCheck()) return false;

    if (pieceCode==6){
        if (col2==6){ //kingside
            if (!board.whiteCastleKing) return false;
            completeMove toCheck1(board,0,4,0,5);
            if (!toCheck1.isLegal()) return false;
            completeMove toCheck2(toCheck1.getNewBoard(),0,5,0,6);
            toCheck2.board.turnColor = board.turnColor;
            if (!toCheck2.isLegal()) return false;
            return true;
        }else if (col2==2){
            if (!board.whiteCastleQueen) return false;
            if (isObstructed(board[0][1])) return false;
            completeMove toCheck1(board,0,4,0,3);
            if (!toCheck1.isLegal()) return false;
            completeMove toCheck2(toCheck1.getNewBoard(),0,3,0,2);
            toCheck2.board.turnColor = board.turnColor;
            if (!toCheck2.isLegal()) return false;
            return true;
        }
    }else{
        BOOST_ASSERT_MSG(pieceCode==-6, "Castle without king move");

        if (col2==6){ //kingside
            if (!board.blackCastleKing) return false;
            completeMove toCheck1(board,7,4,7,5);
            if (!toCheck1.isLegal()) return false;
            completeMove toCheck2(toCheck1.getNewBoard(),7,5,7,6);
            toCheck2.board.turnColor = board.turnColor;
            if (!toCheck2.isLegal()) return false;
            return true;
        }else if (col2==2){
            if (!board.blackCastleQueen) return false;
            if (isObstructed(board[7][1])) return false;
            completeMove toCheck1(board,7,4,7,3);
            if (!toCheck1.isLegal()) return false;
            completeMove toCheck2(toCheck1.getNewBoard(),7,3,7,2);
            toCheck2.board.turnColor = board.turnColor;
            if (!toCheck2.isLegal()) return false;
            return true;
        }
    }


}

bool completeMove::inCheck() const
{
    int kingRow, kingCol;

    for (int i=0; i<8; ++i){
        bool found = false;
        for (int j=0; j<8; ++j){
            const int pieceId = board[i][j];
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
            const int pieceId = board[i][j];
            if (pieceId*board.turnColor<0){ //enemy piece
                boardMove toCheck(board,i,j,kingRow,kingCol);
                if (toCheck.isLegal()) return false;
            }
        }
    }

    return true;
}

completeMove::completeMove(const position &thePosition, const int theRow1, const int theCol1, const int theRow2, const int theCol2):
    boardMove(thePosition, theRow1, theCol1, theRow2, theCol2)
{
}

bool completeMove::isLegal() const
{
    if (newBoard.wasCastle) return handleCastle();

    if (!boardMove::isLegal()) return false;

    return isCheckSafe();
}

position completeMove::getNewBoard() const
{
    return newBoard;
}
