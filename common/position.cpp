#include "position.h"
#include <boost/assert.hpp>

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

position::position(const position &givenPos, const Move& move):
    whiteCastleQueen(givenPos.whiteCastleQueen),
    whiteCastleKing(givenPos.whiteCastleKing),
    blackCastleQueen(givenPos.blackCastleQueen),
    blackCastleKing(givenPos.blackCastleKing),
    wasCastle(false),
    wasEnPassant(false),
    wasPromotion(false),
    turnColor(!givenPos.turnColor)
{
    //makes a new position out of the given one, moving the piece on first to square to second    

    for (int i=0; i<8; ++i){
        for (int j=0; j<8; ++j){
            const Unit givenPiece = givenPos({i,j});
            if (givenPiece.piece == Piece::Shadow) cells[i][j] = {Color::None, Piece::None}; //clear shadow pawn
            else cells[i][j] = givenPiece;
        }
    }

    const Unit pieceCode = cells[move.square_1.row][move.square_1.col];
    const Unit destPiece = givenPos(move.square_2);

    //check if en passant capture
    if (destPiece.piece == Piece::Shadow){ //about to nick a shadow pawn
        wasEnPassant = true;
        cells[move.square_2.row+sign(turnColor)][move.square_2.col] = {Color::None, Piece::None}; //remove the true pawn
    }

    //make the move
    cells[move.square_2.row][move.square_2.col] = pieceCode;
    cells[move.square_1.row][move.square_1.col] = {Color::None, Piece::None};

    //this will perform the castle regardless of whether the side has castling rights
    //will produce gibberish if castling rights have been lost or way is obstructed
    if (pieceCode == Unit{Color::White, Piece::Rook}){
        if ((move.square_1.row==0)&&(move.square_2.row==0)){
            if (move.square_1.col==4){
                if (move.square_2.col==6){
                    cells[0][5] = cells[0][7];
                    cells[0][7] = {Color::None, Piece::None};
                    wasCastle = true;
                }else if (move.square_2.col==2){
                    cells[0][3] = cells[0][0];
                    cells[0][0] = {Color::None, Piece::None};
                    wasCastle = true;
                }
            }
        }
    }else if (pieceCode == Unit{Color::Black, Piece::Rook}){
        if ((move.square_1.row==7)&&(move.square_2.row==7)){
            if (move.square_1.col==4){
                if (move.square_2.col==6){
                    cells[7][5] = cells[7][7];
                    cells[7][7] = {Color::None, Piece::None};
                    wasCastle = true;
                }else if (move.square_2.col==2){
                    cells[7][3] = cells[7][0];
                    cells[7][0] = {Color::None, Piece::None};
                    wasCastle = true;
                }
            }
        }
    }

    //checking if castling rights should be lost
    if (pieceCode.color == Color::White){
        if (pieceCode.piece == Piece::King){
            whiteCastleQueen = false;
            whiteCastleKing = false;
        }else if (pieceCode.piece == Piece::Rook){
            if ((move.square_1.row==0)&&(move.square_1.col==0)) whiteCastleQueen = false;
            else if ((move.square_1.row==0)&&(move.square_1.col==7)) whiteCastleKing = false;
        }
    }else if(pieceCode.color == Color::Black){
        if (pieceCode.piece == Piece::King){
            blackCastleQueen = false;
            blackCastleKing = false;
        }else if (pieceCode.piece == Piece::Rook){
            if ((move.square_1.row==7)&&(move.square_1.col==0)) blackCastleQueen = false;
            else if ((move.square_1.row==7)&&(move.square_1.col==7)) blackCastleKing = false;
        }
    }


    //create shadow pawn for en passant
    if (pieceCode == Unit{Color::White, Piece::Pawn}){
        if ((move.square_1.row==1)&&(move.square_2.row==3))
            cells[2][move.square_2.col] = {Color::White, Piece::Shadow};
    }else if (pieceCode == Unit{Color::Black, Piece::Pawn}){
        if ((move.square_1.row==6)&&(move.square_2.row==4))
            cells[5][move.square_2.col] = {Color::Black, Piece::Shadow};
    }

    //check if promotion
    if (pieceCode == Unit{Color::White, Piece::Pawn}){
        if (move.square_2.row==7){
            wasPromotion = true;            
        }
    }else if (pieceCode == Unit{Color::Black, Piece::Pawn}){
        if (move.square_2.row==0){
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

    cells[0][0] = {Color::White, Piece::Rook};
    cells[0][1] = {Color::White, Piece::Knight};
    cells[0][2] = {Color::White, Piece::Bishop};
    cells[0][3] = {Color::White, Piece::Queen};
    cells[0][4] = {Color::White, Piece::King};
    cells[0][5] = {Color::White, Piece::Bishop};
    cells[0][6] = {Color::White, Piece::Knight};
    cells[0][7] = {Color::White, Piece::Rook};
    cells[7][0] = {Color::Black, Piece::Rook};
    cells[7][1] = {Color::Black, Piece::Knight};
    cells[7][2] = {Color::Black, Piece::Bishop};
    cells[7][3] = {Color::Black, Piece::Queen};
    cells[7][4] = {Color::Black, Piece::King};
    cells[7][5] = {Color::Black, Piece::Bishop};
    cells[7][6] = {Color::Black, Piece::Knight};
    cells[7][7] = {Color::Black, Piece::Rook};

    for (int i=0; i<8; ++i){
        cells[1][i] = {Color::White, Piece::Pawn};
        cells[6][i] = {Color::Black, Piece::Pawn};
        for (int j=2; j<6; ++j){
            cells[j][i] = {Color::None, Piece::None};
        }
    }

}

void position::setPromotion(const Square& square, const Unit& chosenPiece)
{
    BOOST_ASSERT_MSG((square.row>=0)&&(square.row<8)&&
                     (square.col>=0)&&(square.col<8), "Invalid square");

    cells[square.row][square.col] = chosenPiece;
}

Color position::getTurnColor() const
{
    return turnColor;
}

void position::setTurnColor(Color color)
{
    turnColor = color;
}

const Unit& position::operator ()(const Square& square) const
{
    BOOST_ASSERT_MSG((square.row>=0)&&(square.row<8)&&
                     (square.col>=0)&&(square.col<8), "Invalid square");

    return cells[square.row][square.col];
}
