#include "position.h"

position::position()
{
    init();
}

position::position(const position &givenPos, const int row1, const int col1, const int row2, const int col2):
    turnColor(-givenPos.turnColor)
{
    //makes a new position out of the given one, moving the piece on first to square to second
    //must add functionality for castling and en passant

    for (int i=0; i<8; ++i){
        for (int j=0; j<8; ++j){
            cells[i][j] = givenPos[i][j];
        }
    }

    const int pieceCode = cells[row1][row2];

    cells[row2][col2] = cells[row1][col1];
    cells[row1][col1] = 0;

    //this will perform the castle regardless of whether the side has castling rights
    //will produce gibberish if castling rights have been lost or way is obstructed
    if (pieceCode==6){
        if ((row1==0)&&(row2==0)){
            if (col1==4){
                if (col2==6){
                    cells[0][5] = cells[0][7];
                    cells[0][7] = 0;
                }else if (col2==2){
                    cells[0][3] = cells[0][0];
                    cells[0][0] = 0;
                }
            }
        }
    }else if (pieceCode==-6){
        if ((row1==7)&&(row2==7)){
            if (col1==4){
                if (col2==6){
                    cells[7][5] = cells[7][7];
                    cells[7][7] = 0;
                }else if (col2==2){
                    cells[7][3] = cells[7][0];
                    cells[7][0] = 0;
                }
            }
        }
    }

}


void position::init()
{
    turnColor = 1;

    whiteCastleQueen = true;
    whiteCastleKing = true;
    blackCastleQueen = true;
    blackCastleKing = true;

    cells[0][0] = 1;
    cells[0][1] = 3;
    cells[0][2] = 2;
    cells[0][3] = 4;
    cells[0][4] = 6;
    cells[0][5] = 2;
    cells[0][6] = 3;
    cells[0][7] = 1;
    cells[7][0] = -1;
    cells[7][1] = -3;
    cells[7][2] = -2;
    cells[7][3] = -4;
    cells[7][4] = -6;
    cells[7][5] = -2;
    cells[7][6] = -3;
    cells[7][7] = -1;

    for (int i=0; i<8; ++i){
        cells[1][i] = 5;
        cells[6][i] = -5;
        for (int j=2; j<6; ++j){
            cells[j][i] = 0;
        }
    }

}
