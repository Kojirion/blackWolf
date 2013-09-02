//describes a position
//can be constructed given another position and a move to make on it
//thus it is aware if move just made was castling, en passant or promotion
#ifndef POSITION_H
#define POSITION_H
#include <vector>
#include <array>
//#include "blackWolf.h"

class position{
public:

    position();

    position (int boardArray[8][8]);

    position(const position &givenPos, const int row1, const int col1, const int row2, const int col2);

    class positionRow{
    private:
        const position &parent;
        int rowId;

    public:
        positionRow(const position &p, int theRowId):
            parent(p), rowId(theRowId)
        {

        }

        const int& operator[](int colId)  const {
            return parent.pget(rowId,colId);
        }
    };

    positionRow operator[](int rowId) const{
        return positionRow(*this, rowId);
    }

    int turnColor;
    bool whiteCastleQueen;
    bool whiteCastleKing;
    bool blackCastleQueen;
    bool blackCastleKing;

    bool wasCastle;
    bool wasEnPassant;
    bool wasPromotion;

    void setPromotion(const int row, const int col, const int chosenPiece);

private:
    std::array<std::array<int, 8>, 8> cells;

    friend class positionRow;
    const int&   pget(int row,int col) const{
        return cells[row][col];
    }

    void init();


};

#endif // POSITION_H
