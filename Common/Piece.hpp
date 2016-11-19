#pragma once
#include "Color.hpp"

struct Piece{
    enum class Type{
        Rook = 0,
        Knight,
        Bishop,
        Queen,
        King,
        Pawn,
        Shadow,
        None
    };

    Color color;
    Type type;
};

bool operator==(const Piece& lhs, const Piece& rhs);
