#include "Piece.hpp"

bool operator ==(const Piece &lhs, const Piece &rhs)
{
    return (lhs.color == rhs.color) && (lhs.type == rhs.type);
}
