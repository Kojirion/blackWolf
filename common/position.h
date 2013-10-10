//describes a position
//can be constructed given another position and a move to make on it
//thus it is aware if move just made was castling, en passant or promotion
#ifndef POSITION_H
#define POSITION_H
#include <vector>
#include <array>
#include "../blackWolf.h"

class position{
public:

    position();

    position (int boardArray[8][8]);

    position(const position &givenPos, int row1, int col1, int row2, int col2);

    bw operator()(int row, int col) const;


    bool whiteCastleQueen;
    bool whiteCastleKing;
    bool blackCastleQueen;
    bool blackCastleKing;

    bool wasCastle;
    bool wasEnPassant;
    bool wasPromotion;

    void setPromotion(int row, int col, bw chosenPiece);

    bw getTurnColor() const;
    void setTurnColor(bw color);

private:
    bw turnColor;

    std::array<std::array<bw, 8>, 8> cells;




    void init();


};

#endif // POSITION_H
