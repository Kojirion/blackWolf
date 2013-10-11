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
    turnColor(!givenPos.turnColor),
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
            const Piece givenPiece = givenPos(i,j);
            if (givenPiece.type == Type::Shadow) cells[i][j] = {Color::None, Type::None}; //clear shadow pawn
            else cells[i][j] = givenPiece;
        }
    }

    const Piece pieceCode = cells[row1][col1];
    const Piece destPiece = givenPos(row2, col2);

    //check if en passant capture
    if (destPiece.type == Type::Shadow){ //about to nick a shadow pawn
        wasEnPassant = true;
        cells[row2+sign(turnColor)][col2] = {Color::None, Type::None}; //remove the true pawn
    }

    //make the move
    cells[row2][col2] = pieceCode;
    cells[row1][col1] = {Color::None, Type::None};

    //this will perform the castle regardless of whether the side has castling rights
    //will produce gibberish if castling rights have been lost or way is obstructed
    if (pieceCode == Piece{Color::White, Type::Rook}){
        if ((row1==0)&&(row2==0)){
            if (col1==4){
                if (col2==6){
                    cells[0][5] = cells[0][7];
                    cells[0][7] = {Color::None, Type::None};
                    wasCastle = true;
                }else if (col2==2){
                    cells[0][3] = cells[0][0];
                    cells[0][0] = {Color::None, Type::None};
                    wasCastle = true;
                }
            }
        }
    }else if (pieceCode == Piece{Color::Black, Type::Rook}){
        if ((row1==7)&&(row2==7)){
            if (col1==4){
                if (col2==6){
                    cells[7][5] = cells[7][7];
                    cells[7][7] = {Color::None, Type::None};
                    wasCastle = true;
                }else if (col2==2){
                    cells[7][3] = cells[7][0];
                    cells[7][0] = {Color::None, Type::None};
                    wasCastle = true;
                }
            }
        }
    }

    //checking if castling rights should be lost
    if (pieceCode.color == Color::White){
        if (pieceCode.type == Type::King){
            whiteCastleQueen = false;
            whiteCastleKing = false;
        }else if (pieceCode.type == Type::Rook){
            if ((row1==0)&&(col1==0)) whiteCastleQueen = false;
            else if ((row1==0)&&(col1==7)) whiteCastleKing = false;
        }
    }else if(pieceCode.color == Color::Black){
        if (pieceCode.type == Type::King){
            blackCastleQueen = false;
            blackCastleKing = false;
        }else if (pieceCode.type == Type::Rook){
            if ((row1==7)&&(col1==0)) blackCastleQueen = false;
            else if ((row1==7)&&(col1==7)) blackCastleKing = false;
        }
    }


    //create shadow pawn for en passant
    if (pieceCode == Piece{Color::White, Type::Pawn}){
        if ((row1==1)&&(row2==3)) cells[2][col2] = {Color::White, Type::Shadow};
    }else if (pieceCode == Piece{Color::Black, Type::Pawn}){
        if ((row1==6)&&(row2==4)) cells[5][col2] = {Color::Black, Type::Shadow};
    }

    //check if promotion
    if (pieceCode == Piece{Color::White, Type::Pawn}){
        if (row2==7){
            wasPromotion = true;            
        }
    }else if (pieceCode == Piece{Color::Black, Type::Pawn}){
        if (row2==0){
            wasPromotion = true;            
        }
    }

}


void position::init()
{
    turnColor = Color::White;

    whiteCastleQueen = true;
    whiteCastleKing = true;
    blackCastleQueen = true;
    blackCastleKing = true;

    wasCastle = false;
    wasEnPassant = false;
    wasPromotion = false;

    cells[0][0] = {Color::White, Type::Rook};
    cells[0][1] = {Color::White, Type::Knight};
    cells[0][2] = {Color::White, Type::Bishop};
    cells[0][3] = {Color::White, Type::Queen};
    cells[0][4] = {Color::White, Type::King};
    cells[0][5] = {Color::White, Type::Bishop};
    cells[0][6] = {Color::White, Type::Knight};
    cells[0][7] = {Color::White, Type::Rook};
    cells[7][0] = {Color::Black, Type::Rook};
    cells[7][1] = {Color::Black, Type::Knight};
    cells[7][2] = {Color::Black, Type::Bishop};
    cells[7][3] = {Color::Black, Type::Queen};
    cells[7][4] = {Color::Black, Type::King};
    cells[7][5] = {Color::Black, Type::Bishop};
    cells[7][6] = {Color::Black, Type::Knight};
    cells[7][7] = {Color::Black, Type::Rook};

    for (int i=0; i<8; ++i){
        cells[1][i] = {Color::White, Type::Pawn};
        cells[6][i] = {Color::Black, Type::Pawn};
        for (int j=2; j<6; ++j){
            cells[j][i] = {Color::None, Type::None};
        }
    }

}

void position::setPromotion(const int row, const int col, Piece chosenPiece)
{
    cells[row][col] = chosenPiece;
}

Color position::getTurnColor() const
{
    return turnColor;
}

void position::setTurnColor(Color color)
{
    turnColor = color;
}

const Piece& position::operator ()(int row, int col) const
{
    //asserts here
    return cells[row][col];
}
