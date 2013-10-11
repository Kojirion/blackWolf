#ifndef BLACKWOLF_H
#define BLACKWOLF_H

enum class Color{
    None = 0,
    White = 1 << 0,
    Black = 1 << 1,
    Both = White | Black
};

enum class Type{
    None,
    Pawn,
    Rook,
    Knight,
    Bishop,
    Queen,
    King,
    Shadow
};

struct Piece{
    Color color;
    Type type;
};

const Piece noPiece = {Color::None, Type::None};

bool operator==(const Piece& lhs, const Piece& rhs);
bool operator &(const Color& lhs, const Color& rhs);

Color operator!(Color rhs);

int sign(Color color);

//#include "blackWolf.inl"

#endif // BLACKWOLF_H
