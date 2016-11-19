#include "BlackWolf.hpp"
#include <cassert>
#include "Common/Piece.hpp"

















sf::Vector2f PieceToTexPos::operator()(const Piece &piece) const
{
    assert((piece.color==Color::Black)||(piece.color==Color::White));
    assert((piece.type!=Piece::Type::None)&&(piece.type!=Piece::Type::Shadow));

    const int side = 50;

    if (piece.color == Color::White)
        return sf::Vector2f(static_cast<int>(piece.type)*side, whiteIndex*side);
    else
        return sf::Vector2f(static_cast<int>(piece.type)*side, blackIndex*side);
}
