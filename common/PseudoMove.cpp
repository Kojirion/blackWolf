#include "PseudoMove.hpp"
#include <boost/math/special_functions/sign.hpp>
#include <boost/assert.hpp>
#include <cmath>

PseudoMove::PseudoMove(const Position &thePosition, const Move& move):
    m_board(thePosition),
    m_move(move),
    newBoard(thePosition, move)
{    
    m_piece = m_board(m_move.square_1);
    BOOST_ASSERT_MSG(m_piece.piece != Piece::None, "No piece in starting square");
    BOOST_ASSERT_MSG(m_piece.piece != Piece::Shadow, "Shadow pawn in starting square");
}

bool PseudoMove::isLegal() const
{
    if (startEndSame()) return false;

    if (isOccupied()) return false;

    if (m_piece.piece == Piece::Rook)
        return isRookLegal();
    if (m_piece.piece == Piece::Bishop)
        return isBishopLegal();
    if (m_piece.piece == Piece::Knight)
       return isKnightLegal();
    if (m_piece.piece == Piece::Queen)
        return isQueenLegal();
    if (m_piece.piece == Piece::Pawn)
        return isPawnLegal();
    if (m_piece.piece == Piece::King)
        return isKingLegal();

    BOOST_ASSERT_MSG(m_piece.piece == Piece::Shadow, "Invalid piececode");

    return false;

}


bool PseudoMove::startEndSame() const
{
    return ((m_move.square_1.row==m_move.square_2.row)&&(m_move.square_1.col==m_move.square_2.col));
}

bool PseudoMove::isOccupied() const
{   
    return (m_piece.color == m_board(m_move.square_2).color);
}

bool PseudoMove::isKnightLegal() const
{
    for (int i=-2; i<3; i++){
        for (int j=-2; j<3; j++){
            if ((i!=0)&&(j!=0)&&(std::abs(i) != std::abs(j))){
                if ((m_move.square_1.row+i == m_move.square_2.row)&&(m_move.square_1.col+j == m_move.square_2.col)) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool PseudoMove::isBishopLegal() const
{

    const int deltaRow = m_move.square_2.row - m_move.square_1.row;
    const int deltaCol = m_move.square_2.col - m_move.square_1.col;

    //if not in same diagonal, do not proceed
    if (std::abs(deltaRow) != std::abs(deltaCol)) return false;

    const int signRowDiff = boost::math::sign(deltaRow);
    const int signColDiff = boost::math::sign(deltaCol);

    for (int i=m_move.square_1.row+signRowDiff; i!=m_move.square_2.row; i+=signRowDiff){
        int j = m_move.square_1.col + (i-m_move.square_1.row)*signColDiff*signRowDiff;
        if (isObstructed(m_board({i, j}))) return false;
    }
    return true;
}

bool PseudoMove::isRookLegal() const
{
    if (m_move.square_1.row==m_move.square_2.row){
        const int signDiff = boost::math::sign(m_move.square_2.col - m_move.square_1.col);
        const int maxIt = m_move.square_2.col - m_move.square_1.col;

        for (int i=signDiff; i!=maxIt; i+=signDiff){
            if (isObstructed(m_board({m_move.square_1.row, m_move.square_1.col+i}))) return false;
        }
        return true; //no obstruction
    }else if (m_move.square_1.col==m_move.square_2.col){
        const int signDiff = boost::math::sign(m_move.square_2.row - m_move.square_1.row);
        const int maxIt = m_move.square_2.row - m_move.square_1.row;

        for (int i=signDiff; i!=maxIt; i+=signDiff){
            if (isObstructed(m_board({m_move.square_1.row+i, m_move.square_1.col}))) return false;
        }
        return true; //no obstruction
    }
    return false; //not in same row nor column
}

bool PseudoMove::isQueenLegal() const
{
    return (isBishopLegal() || isRookLegal());
}

bool PseudoMove::isKingLegal() const
{
    if (newBoard.wasCastle) return true; //tentative true
    //completeMove will ultimately decide if the castle is legal

    if ((std::abs(m_move.square_2.row-m_move.square_1.row)>1) || (std::abs(m_move.square_2.col-m_move.square_1.col)>1)) return false;

    return isQueenLegal();
}

bool PseudoMove::isPawnLegal() const
{
    //can be rewritten much smaller

    if (m_board(m_move.square_1).color == Color::White){ //white pawn
        if ((m_move.square_1.row==1)&&(m_move.square_2.row==3)&&(m_move.square_1.col==m_move.square_2.col)){ //double advance
            if ((m_board(m_move.square_2) == noPiece) && (m_board({2, m_move.square_2.col}) == noPiece))
                return true;
        }else{
            if (m_move.square_1.row+1==m_move.square_2.row){
                if (m_move.square_1.col==m_move.square_2.col)
                    if (m_board(m_move.square_2) == noPiece) return true;
                if ((m_move.square_1.col+1==m_move.square_2.col)||(m_move.square_1.col-1==m_move.square_2.col))
                    if (m_piece.color == !m_board(m_move.square_2).color) return true;
            }
        }
    }else{
        if ((m_move.square_1.row==6)&&(m_move.square_2.row==4)&&(m_move.square_1.col==m_move.square_2.col)){ //double advance
            if ((m_board(m_move.square_2) == noPiece) && (m_board({5, m_move.square_2.col}) == noPiece))
                return true;
        }else{
            if (m_move.square_1.row-1==m_move.square_2.row){
                if (m_move.square_1.col==m_move.square_2.col)
                    if (m_board(m_move.square_2) == noPiece) return true;
                if ((m_move.square_1.col+1==m_move.square_2.col)||(m_move.square_1.col-1==m_move.square_2.col))
                    if (m_piece.color == !m_board(m_move.square_2).color) return true;
            }
        }
    }
    return false;
}

bool PseudoMove::isObstructed(const Unit &unit) const
{
    return (unit.piece != Piece::None) && (unit.piece != Piece::Shadow);
}
