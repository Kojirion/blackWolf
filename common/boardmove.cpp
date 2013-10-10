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
    pieceCode = board(row1, col1);
    BOOST_ASSERT_MSG(!check(pieceCode & bw::None), "No piece in starting square");
    BOOST_ASSERT_MSG(!check(pieceCode & bw::Shadow), "Shadow pawn in starting square");
}

bool boardMove::isLegal() const
{
    if (startEndSame()) return false;

    if (isOccupied()) return false;

    if (check(pieceCode & bw::Rook))
        return isRookLegal();
    if (check(pieceCode & bw::Bishop))
        return isBishopLegal();
    if (check(pieceCode & bw::Knight))
       return isKnightLegal();
    if (check(pieceCode & bw::Queen))
        return isQueenLegal();
    if (check(pieceCode & bw::Pawn))
        return isPawnLegal();
    if (check(pieceCode & bw::King))
        return isKingLegal();

    BOOST_ASSERT_MSG(check(pieceCode & bw::Shadow), "Invalid piececode");

    return false;

}


bool boardMove::startEndSame() const
{
    return ((row1==row2)&&(col1==col2));
}

bool boardMove::isOccupied() const
{
    //checks if occupied by piece of same color
    return (check(pieceCode & board(row2, col2)));
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

    if (check(board(row1, col1) & bw::White)){ //white pawn
        if ((row1==1)&&(row2==3)&&(col1==col2)){ //double advance
            if (check(board(row2, col2) & bw::None) && check(board(2, col2) & bw::None))
                return true;
        }else{
            if (row1+1==row2){
                if (col1==col2)
                    if (check(board(row2, col2) & bw::None)) return true;
                if ((col1+1==col2)||(col1-1==col2))
                    if (check(pieceCode & board(row2, col2))) return true;
            }
        }
    }else{
        if ((row1==6)&&(row2==4)&&(col1==col2)){ //double advance
            if (check(board(row2, col2) & bw::None) && check(board(5, col2) & bw::None))
                return true;
        }else{
            if (row1-1==row2){
                if (col1==col2)
                    if (check(board(row2, col2) & bw::None)) return true;
                if ((col1+1==col2)||(col1-1==col2))
                    if (check(pieceCode & board(row2, col2))) return true;
            }
        }
    }
    return false;
}

bool boardMove::isObstructed(bw pieceCode) const
{
    return (!check(pieceCode & bw::None) && check(pieceCode & bw::Shadow));
}
