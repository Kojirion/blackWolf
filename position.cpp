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

    cells[row2][col2] = cells[row1][col1];
    cells[row1][col1] = 0;

}

void position::init()
{
    turnColor = 1;

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
