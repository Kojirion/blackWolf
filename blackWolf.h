#ifndef BLACKWOLF_H
#define BLACKWOLF_H

enum class Color{
    None = 0,
    White = 1 << 0,
    Black = 1 << 1,
    Both = White | Black
};

enum class Piece{
    None,
    Pawn,
    Rook,
    Knight,
    Bishop,
    Queen,
    King,
    Shadow
};

struct Unit{
    Color color;
    Piece type;
};

const Unit noPiece = {Color::None, Piece::None};

bool operator==(const Unit& lhs, const Unit& rhs);
bool operator &(const Color& lhs, const Color& rhs);

Color operator!(Color rhs);

int sign(Color color);

//#include "blackWolf.inl"

#endif // BLACKWOLF_H
