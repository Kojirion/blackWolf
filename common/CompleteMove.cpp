#include "CompleteMove.hpp"
#include <boost/assert.hpp>
#include <map>

//maps the castle to the square the king has to go through
static const std::map<Castle, Square> castlingMap = {
    {{Color::White, Side::King}, {0,5}},
    {{Color::White, Side::Queen}, {0,3}},
    {{Color::Black, Side::King}, {7,5}},
    {{Color::Black, Side::Queen}, {7,3}}
};


bool CompleteMove::isCheckSafe() const
{
    return !inCheck(newBoard, m_board.getTurnColor());
}

bool CompleteMove::handleCastle() const
{
    BOOST_ASSERT_MSG((m_piece.piece == Piece::King), "Castle without king move");
    BOOST_ASSERT_MSG((m_piece.color == m_board.getTurnColor()), "Opponent requests castle");

    if (inCheck(m_board, m_board.getTurnColor()))
        return false;

    Castle castle = getCastle();

    if (!m_board.castlingRights(castle))
        return false;

    const Square& square = castlingMap.at(castle);
    CompleteMove move_1(m_board, {m_move.square_1, square});
    if (!move_1.isLegal())
        return false;
    CompleteMove move_2(move_1.getNewBoard(), {square, m_move.square_2});
    return move_2.isLegalColorblind();
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

bool CompleteMove::isLegalColorblind() const
{
    if (newBoard.wasCastle)
        return handleCastle();

    if (!PseudoMove::isLegal())
        return false;

    return isCheckSafe();
}

Castle CompleteMove::getCastle() const
{
    Color color = m_piece.color;
    Side side;
    if (m_move.square_2.col==6)
        side = Side::King;
    else {
        BOOST_ASSERT_MSG (m_move.square_2.col == 2, "Invalid square to castle to");
        side = Side::Queen;
    }
    return {color, side};
}

CompleteMove::CompleteMove(const Position &thePosition, const Move &move):
    PseudoMove(thePosition, move),
    fiftyMoveValid((m_piece.piece != Piece::Pawn)&&(m_board(m_move.square_2) == Unit{Color::None, Piece::None}))
{    
}

bool CompleteMove::isLegal() const
{
    if (m_piece.color != m_board.getTurnColor())
        return false;

    return isLegalColorblind();

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
