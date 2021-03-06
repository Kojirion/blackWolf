#include "BlackWolf.hpp"
#include <cassert>

int sign(Color color)
{
    if (color == Color::White) return 1;
    assert(color == Color::Black);
    return -1;
}


Color operator !(Color rhs)
{
    if (rhs==Color::White) return Color::Black;
    if (rhs==Color::Black) return Color::White;
    assert(true);
    return rhs;
}


bool operator ==(const Piece &lhs, const Piece &rhs)
{
    return (lhs.color == rhs.color) && (lhs.type == rhs.type);
}


bool operator &(const Color &lhs, const Color &rhs)
{
    return static_cast<int>(lhs) & static_cast<int>(rhs);
}


bool operator <(const Square &lhs, const Square &rhs)
{
    if(lhs.row==rhs.row) return (lhs.col<rhs.col);
    return (lhs.row<rhs.row);
}


std::ostream &operator<<(std::ostream &stream, const Square &square)
{
    static const char* colToChar = "abcdefgh";
    stream << colToChar[square.col] << (square.row+1);
    return stream;
}


std::ostream &operator<<(std::ostream &stream, const Move &move)
{
    stream << move.square_1 << move.square_2;
    return stream;
}


bool operator==(const Square &lhs, const Square &rhs)
{
    return (lhs.row == rhs.row) && (lhs.col == rhs.col);
}


bool operator==(const Move &lhs, const Move &rhs)
{
    return (lhs.square_1 == rhs.square_1) && (lhs.square_2 == rhs.square_2);
}

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
