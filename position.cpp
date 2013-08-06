#include "position.h"

position::position()
{
}

position::position(const position &givenPos, const int row1, const int col1, const int row2, const int col2)
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
