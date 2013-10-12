#include "boardmove.h"
#include <boost/math/special_functions/sign.hpp>
#include <boost/assert.hpp>
#include <cmath>

boardMove::boardMove(const position &thePosition, const int theRow1, const int theCol1, const int theRow2, const int theCol2):
    board(thePosition),
    row1(theRow1),
    col1(theCol1),
    row2(theRow2),
    col2(theCol2),
    newBoard(thePosition,theRow1,theCol1,theRow2,theCol2)
{    
    m_piece = board(row1, col1);
    BOOST_ASSERT_MSG(m_piece.piece != Piece::None, "No piece in starting square");
    BOOST_ASSERT_MSG(m_piece.piece != Piece::Shadow, "Shadow pawn in starting square");
}

bool boardMove::isLegal() const
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


bool boardMove::startEndSame() const
{
    return ((row1==row2)&&(col1==col2));
}

bool boardMove::isOccupied() const
{   
    return (m_piece.color == board(row2, col2).color);
}

bool boardMove::isKnightLegal() const
{
    for (int i=-2; i<3; i++){
        for (int j=-2; j<3; j++){
            if ((i!=0)&&(j!=0)&&(std::abs(i) != std::abs(j))){
                if ((row1+i == row2)&&(col1+j == col2)) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool boardMove::isBishopLegal() const
{

    const int deltaRow = row2 - row1;
    const int deltaCol = col2 - col1;

    //if not in same diagonal, do not proceed
    if (std::abs(deltaRow) != std::abs(deltaCol)) return false;

    const int signRowDiff = boost::math::sign(deltaRow);
    const int signColDiff = boost::math::sign(deltaCol);

    for (int i=row1+signRowDiff; i!=row2; i+=signRowDiff){
        int j = col1 + (i-row1)*signColDiff*signRowDiff;
        if (isObstructed(board(i, j))) return false;
    }
    return true;
}

bool boardMove::isRookLegal() const
{
    if (row1==row2){
        const int signDiff = boost::math::sign(col2 - col1);
        const int maxIt = col2 - col1;

        for (int i=signDiff; i!=maxIt; i+=signDiff){
            if (isObstructed(board(row1, col1+i))) return false;
        }
        return true; //no obstruction
    }else if (col1==col2){
        const int signDiff = boost::math::sign(row2 - row1);
        const int maxIt = row2 - row1;

        for (int i=signDiff; i!=maxIt; i+=signDiff){
            if (isObstructed(board(row1+i, col1))) return false;
        }
        return true; //no obstruction
    }
    return false; //not in same row nor column
}

bool boardMove::isQueenLegal() const
{
    return (isBishopLegal() || isRookLegal());
}

bool boardMove::isKingLegal() const
{
    if (newBoard.wasCastle) return true; //tentative true
    //completeMove will ultimately decide if the castle is legal

    if ((std::abs(row2-row1)>1) || (std::abs(col2-col1)>1)) return false;

    return isQueenLegal();
}

bool boardMove::isPawnLegal() const
{
    //can be rewritten much smaller

    if (board(row1, col1).color == Color::White){ //white pawn
        if ((row1==1)&&(row2==3)&&(col1==col2)){ //double advance
            if ((board(row2, col2) == noPiece) && (board(2, col2) == noPiece))
                return true;
        }else{
            if (row1+1==row2){
                if (col1==col2)
                    if (board(row2, col2) == noPiece) return true;
                if ((col1+1==col2)||(col1-1==col2))
                    if (m_piece.color != board(row2, col2).color) return true;
            }
        }
    }else{
        if ((row1==6)&&(row2==4)&&(col1==col2)){ //double advance
            if ((board(row2, col2) == noPiece) && (board(5, col2) == noPiece))
                return true;
        }else{
            if (row1-1==row2){
                if (col1==col2)
                    if (board(row2, col2) == noPiece) return true;
                if ((col1+1==col2)||(col1-1==col2))
                    if (m_piece.color != board(row2, col2).color) return true;
            }
        }
    }
    return false;
}

bool boardMove::isObstructed(Unit pieceCode) const
{
    return (pieceCode.piece != Piece::None) && (pieceCode.piece != Piece::Shadow);
}
