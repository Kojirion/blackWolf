#include "completemove.h"
#include <boost/assert.hpp>

bool completeMove::isCheckSafe() const
{
    int kingRow, kingCol;

    for (int i=0; i<8; ++i){
        bool found = false;
        for (int j=0; j<8; ++j){
            const Unit pieceId = newBoard({i, j});
            if (pieceId == Unit{board.getTurnColor(), Piece::King}){ //found our king
                kingRow = i;
                kingCol = j;
                break;
            }
        }
        if (found) break;
    }

    for (int i=0; i<8; ++i){
        for (int j=0; j<8; ++j){
            const Unit pieceId = newBoard({i, j});
            if (pieceId.color == !board.getTurnColor() && (pieceId.piece != Piece::Shadow)){ //enemy piece
                boardMove toCheck(newBoard,{{i,j},{kingRow,kingCol}});
                if (toCheck.isLegal()) return false;
            }
        }
    }

    return true;
}

bool completeMove::handleCastle() const
{
    if (inCheck(board,board.getTurnColor())) return false;

    if (m_piece == Unit{Color::White, Piece::King}){
        if (m_move.square_2.col==6){ //kingside
            if (!board.whiteCastleKing) return false;
            completeMove toCheck1(board,{{0,4},{0,5}});
            if (!toCheck1.isLegal()) return false;
            completeMove toCheck2(toCheck1.getNewBoard(),{{0,5},{0,6}});
            toCheck2.board.setTurnColor(board.getTurnColor());
            if (!toCheck2.isLegal()) return false;
            return true;
        }else if (m_move.square_2.col==2){
            if (!board.whiteCastleQueen) return false;
            if (isObstructed(board({0, 1}))) return false;
            completeMove toCheck1(board,{{0,4},{0,3}});
            if (!toCheck1.isLegal()) return false;
            completeMove toCheck2(toCheck1.getNewBoard(),{{0,3},{0,2}});
            toCheck2.board.setTurnColor(board.getTurnColor());
            if (!toCheck2.isLegal()) return false;
            return true;
        }
    }else{
        BOOST_ASSERT_MSG((m_piece == Unit{Color::Black, Piece::King}), "Castle without king move");

        if (m_move.square_2.col==6){ //kingside
            if (!board.blackCastleKing) return false;
            completeMove toCheck1(board,{{7,4},{7,5}});
            if (!toCheck1.isLegal()) return false;
            completeMove toCheck2(toCheck1.getNewBoard(),{{7,5},{7,6}});
            toCheck2.board.setTurnColor(board.getTurnColor());
            if (!toCheck2.isLegal()) return false;
            return true;
        }else if (m_move.square_2.col==2){
            if (!board.blackCastleQueen) return false;
            if (isObstructed(board({7, 1}))) return false;
            completeMove toCheck1(board,{{7,4},{7,3}});
            if (!toCheck1.isLegal()) return false;
            completeMove toCheck2(toCheck1.getNewBoard(),{{7,3},{7,2}});
            toCheck2.board.setTurnColor(board.getTurnColor());
            if (!toCheck2.isLegal()) return false;
            return true;
        }
    }
    return false; //appease compiler
}

bool completeMove::inCheck(const position &givenPos, Color side) const
{
    int kingRow, kingCol;

    for (int i=0; i<8; ++i){
        bool found = false;
        for (int j=0; j<8; ++j){
            const Unit pieceId = givenPos({i, j});
            if (pieceId == Unit{board.getTurnColor(), Piece::King}){ //found our king
                kingRow = i;
                kingCol = j;
                break;
            }
        }
        if (found) break;
    }

    for (int i=0; i<8; ++i){
        for (int j=0; j<8; ++j){
            const Unit pieceId = givenPos({i, j});
            if (pieceId.color != board.getTurnColor()){ //enemy piece
                boardMove toCheck(givenPos,{{i,j},{kingRow,kingCol}});
                if (toCheck.isLegal()) return true;
            }
        }
    }

    return false;
}

bool completeMove::hasLegalMoves() const
{
    for (int i=0; i<8; ++i){
        for (int j=0; j<8; ++j){
            for (int k=0; k<8; ++k){
                for (int l=0; l<8; ++l){
                    if (newBoard({i, j}).color == newBoard.getTurnColor()){
                        completeMove toCheck(newBoard,{{i,j},{k,l}});
                        if (toCheck.isLegal()) return true;
                    }
                }
            }
        }
    }
    return false;
}

completeMove::completeMove(const position &thePosition, const Move &move):
    boardMove(thePosition, move)
{
}

bool completeMove::isLegal() const
{
    if (m_piece.color != board.getTurnColor()) return false;
    //assumes that color has been switched if necessary

    if (newBoard.wasCastle) return handleCastle();

    if (!boardMove::isLegal()) return false;

    return isCheckSafe();
}

bool completeMove::isCheckmate() const
{
    return ((!hasLegalMoves())&&(inCheck(newBoard,newBoard.getTurnColor())));
}

bool completeMove::isStalemate() const
{
    return ((!hasLegalMoves())&&(!inCheck(newBoard,newBoard.getTurnColor())));
}

int completeMove::getRow1() const
{
    return m_move.square_1.row;
}

int completeMove::getCol1() const
{
    return m_move.square_1.col;
}

int completeMove::getRow2() const
{
    return m_move.square_2.row;
}

int completeMove::getCol2() const
{
    return m_move.square_2.col;
}

position completeMove::getNewBoard() const
{
    return newBoard;
}
