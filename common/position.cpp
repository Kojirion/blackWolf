#include "position.h"
//#include <boost/assert.hpp>

position::position()
{
    init();
}

position::position(int boardArray[8][8])
{
    init();
    for (int i=0; i<8; ++i)
    {
        for (int j=0; j<8; ++j)
        {
            //cells[i][j] = boardArray[i][j];
        }
    }    
}

position::position(const position &givenPos, int row1, int col1, int row2, int col2):
    turnColor(-givenPos.turnColor),
    whiteCastleQueen(givenPos.whiteCastleQueen),
    whiteCastleKing(givenPos.whiteCastleKing),
    blackCastleQueen(givenPos.blackCastleQueen),
    blackCastleKing(givenPos.blackCastleKing),
    wasCastle(false),
    wasEnPassant(false),
    wasPromotion(false)
{
    //makes a new position out of the given one, moving the piece on first to square to second    

    for (int i=0; i<8; ++i){
        for (int j=0; j<8; ++j){
            const bw givenPiece = givenPos(i,j);
            if (check(givenPiece & bw::Shadow)) cells[i][j] = bw::None; //clear shadow pawn
            else cells[i][j] = givenPiece;
        }
    }

    const bw pieceCode = cells[row1][col1];
    const bw destPiece = givenPos(row2, col2);

    //check if en passant capture
    if (check(destPiece & bw::Shadow)){ //about to nick a shadow pawn
        wasEnPassant = true;
        cells[row2+sign(turnColor)][col2] = bw::None; //remove the true pawn
    }

    //make the move
    cells[row2][col2] = pieceCode;
    cells[row1][col1] = bw::None;

    //this will perform the castle regardless of whether the side has castling rights
    //will produce gibberish if castling rights have been lost or way is obstructed
    if (check(pieceCode & (bw::White | bw::Rook))){
        if ((row1==0)&&(row2==0)){
            if (col1==4){
                if (col2==6){
                    cells[0][5] = cells[0][7];
                    cells[0][7] = bw::None;
                    wasCastle = true;
                }else if (col2==2){
                    cells[0][3] = cells[0][0];
                    cells[0][0] = bw::None;
                    wasCastle = true;
                }
            }
        }
    }else if (check(pieceCode & (bw::Black | bw::Rook))){
        if ((row1==7)&&(row2==7)){
            if (col1==4){
                if (col2==6){
                    cells[7][5] = cells[7][7];
                    cells[7][7] = bw::None;
                    wasCastle = true;
                }else if (col2==2){
                    cells[7][3] = cells[7][0];
                    cells[7][0] = bw::None;
                    wasCastle = true;
                }
            }
        }
    }

    //checking if castling rights should be lost
    switch (pieceCode) {
    case (bw::White | bw::King):
        whiteCastleQueen = false;
        whiteCastleKing = false;
        break;
    case (bw::Black | bw::King):
        blackCastleQueen = false;
        blackCastleKing = false;
        break;
    case (bw::White | bw::Rook):
        if ((row1==0)&&(col1==0)) whiteCastleQueen = false;
        else if ((row1==0)&&(col1==7)) whiteCastleKing = false;
        break;
    case (bw::Black | bw::Rook):
        if ((row1==7)&&(col1==0)) blackCastleQueen = false;
        else if ((row1==7)&&(col1==7)) blackCastleKing = false;
        break;
    }

    //create shadow pawn for en passant
    if (check(pieceCode & (bw::White | bw::Pawn))){
        if ((row1==1)&&(row2==3)) cells[2][col2] = bw::White | bw::Shadow;
    }else if (check(pieceCode & (bw::Black | bw::Pawn))){
        if ((row1==6)&&(row2==4)) cells[5][col2] = bw::Black | bw::Shadow;
    }

    //check if promotion
    if (check(pieceCode & (bw::White | bw::Pawn))){
        if (row2==7){
            wasPromotion = true;            
        }
    }else if (check(pieceCode & (bw::Black | bw::Pawn))){
        if (row2==0){
            wasPromotion = true;            
        }
    }

}


void position::init()
{
    turnColor = bw::None;

    whiteCastleQueen = true;
    whiteCastleKing = true;
    blackCastleQueen = true;
    blackCastleKing = true;

    wasCastle = false;
    wasEnPassant = false;
    wasPromotion = false;

    cells[0][0] = bw::Rook | bw::White;
    cells[0][1] = bw::Knight | bw::White;
    cells[0][2] = bw::Bishop | bw::White;
    cells[0][3] = bw::Queen | bw::White;
    cells[0][4] = bw::King | bw::White;
    cells[0][5] = bw::Bishop | bw::White;
    cells[0][6] = bw::Knight | bw::White;
    cells[0][7] = bw::Rook | bw::White;
    cells[7][0] = bw::Rook | bw::Black;
    cells[7][1] = bw::Knight | bw::Black;
    cells[7][2] = bw::Bishop | bw::Black;
    cells[7][3] = bw::Queen | bw::Black;
    cells[7][4] = bw::King | bw::Black;
    cells[7][5] = bw::Bishop | bw::Black;
    cells[7][6] = bw::Knight | bw::Black;
    cells[7][7] = bw::Rook | bw::Black;

    for (int i=0; i<8; ++i){
        cells[1][i] = bw::Pawn | bw::White;
        cells[6][i] = bw::Pawn | bw::Black;
        for (int j=2; j<6; ++j){
            cells[j][i] = bw::None;
        }
    }

}

void position::setPromotion(const int row, const int col, bw chosenPiece)
{
    cells[row][col] = chosenPiece;
}

bw position::getTurnColor() const
{
    return turnColor;
}

void position::setTurnColor(bw color)
{
    turnColor = color;
}

bw position::operator ()(int row, int col) const
{
    return cells[row][col];
}
