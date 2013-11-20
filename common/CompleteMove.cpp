#include "CompleteMove.hpp"
#include <boost/assert.hpp>

bool CompleteMove::isCheckSafe() const
{
    return !inCheck(newBoard, m_board.getTurnColor());
}

bool CompleteMove::handleCastle() const
{
    if (inCheck(m_board,m_board.getTurnColor())) return false;

    if (m_piece == Unit{Color::White, Piece::King}){
        if (m_move.square_2.col==6){ //kingside
            if (!m_board.whiteCastleKing) return false;
            CompleteMove toCheck1(m_board,{{0,4},{0,5}});
            if (!toCheck1.isLegal()) return false;
            CompleteMove toCheck2(toCheck1.getNewBoard(),{{0,5},{0,6}});
            toCheck2.m_board.setTurnColor(m_board.getTurnColor());
            if (!toCheck2.isLegal()) return false;
            return true;
        }else if (m_move.square_2.col==2){
            if (!m_board.whiteCastleQueen) return false;
            if (isObstructed(m_board({0, 1}))) return false;
            CompleteMove toCheck1(m_board,{{0,4},{0,3}});
            if (!toCheck1.isLegal()) return false;
            CompleteMove toCheck2(toCheck1.getNewBoard(),{{0,3},{0,2}});
            toCheck2.m_board.setTurnColor(m_board.getTurnColor());
            if (!toCheck2.isLegal()) return false;
            return true;
        }
    }else{
        BOOST_ASSERT_MSG((m_piece == Unit{Color::Black, Piece::King}), "Castle without king move");

        if (m_move.square_2.col==6){ //kingside
            if (!m_board.blackCastleKing) return false;
            CompleteMove toCheck1(m_board,{{7,4},{7,5}});
            if (!toCheck1.isLegal()) return false;
            CompleteMove toCheck2(toCheck1.getNewBoard(),{{7,5},{7,6}});
            toCheck2.m_board.setTurnColor(m_board.getTurnColor());
            if (!toCheck2.isLegal()) return false;
            return true;
        }else if (m_move.square_2.col==2){
            if (!m_board.blackCastleQueen) return false;
            if (isObstructed(m_board({7, 1}))) return false;
            CompleteMove toCheck1(m_board,{{7,4},{7,3}});
            if (!toCheck1.isLegal()) return false;
            CompleteMove toCheck2(toCheck1.getNewBoard(),{{7,3},{7,2}});
            toCheck2.m_board.setTurnColor(m_board.getTurnColor());
            if (!toCheck2.isLegal()) return false;
            return true;
        }
    }
    return false; //appease compiler
}

bool CompleteMove::inCheck(const Position &givenPos, Color side) const
{

    auto kingSquare = findKing(givenPos, side);

    for (int i=0; i<8; ++i){
        for (int j=0; j<8; ++j){
            const Unit pieceId = givenPos({i, j});
            if (pieceId.color == !side && (pieceId.piece != Piece::Shadow)){
                PseudoMove toCheck(givenPos,{{i,j}, kingSquare});
                if (toCheck.isLegal()) return true;
            }
        }
    }

    return false;
}

bool CompleteMove::hasLegalMoves() const
{
    for (int i=0; i<8; ++i){
        for (int j=0; j<8; ++j){
            for (int k=0; k<8; ++k){
                for (int l=0; l<8; ++l){
                    if (newBoard({i, j}).color == newBoard.getTurnColor()){
                        CompleteMove toCheck(newBoard,{{i,j},{k,l}});
                        if (toCheck.isLegal()) return true;
                    }
                }
            }
        }
    }
    return false;
}

Square CompleteMove::findKing(const Position &position, Color color) const
{
    for (int i=0; i<8; ++i){
        for (int j=0; j<8; ++j){
            const Unit pieceId = position({i, j});
            if (pieceId == Unit{color, Piece::King})
                return {i,j};
        }
    }
    return {-1, -1};
}

CompleteMove::CompleteMove(const Position &thePosition, const Move &move):
    PseudoMove(thePosition, move)
{
}

bool CompleteMove::isLegal() const
{
    if (m_piece.color != m_board.getTurnColor()) return false;
    //assumes that color has been switched if necessary

    if (newBoard.wasCastle) return handleCastle();

    if (!PseudoMove::isLegal()) return false;

    return isCheckSafe();
}

bool CompleteMove::isCheckmate() const
{
    return ((!hasLegalMoves())&&(inCheck(newBoard,newBoard.getTurnColor())));
}

bool CompleteMove::isStalemate() const
{
    return ((!hasLegalMoves())&&(!inCheck(newBoard,newBoard.getTurnColor())));
}

const Move &CompleteMove::getMove() const
{
    return m_move;
}

Position CompleteMove::getNewBoard() const
{
    return newBoard;
}
