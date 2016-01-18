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


bool operator<(const Castle &lhs, const Castle &rhs)
{
    if (lhs.color != rhs.color)
        return (lhs.color < rhs.color);
    else return (lhs.side < rhs.side);
}


std::ostream &operator<<(std::ostream &stream, const Square &square)
{
    static const char* colToChar = "abcdefgh";
    stream << colToChar[square.col] << (square.row+1);
    return stream;
}


std::ostream &operator<<(std::ostream &stream, const Move &move)
{
    stream << move.square_1 << "-" << move.square_2;
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

sf::Vector2f typeToTexPos(const Piece &type)
{
    assert((type.color==Color::Black)||(type.color==Color::White));
    assert((type.type!=Piece::Type::None)&&(type.type!=Piece::Type::Shadow));


    const sf::Vector2f offset(20.f, 20.f);
    const int side = 50;

    if (type.type == Piece::Type::Pawn){
        if (type.color == Color::White)
            return (offset + sf::Vector2f(0, 6*side));
        else return (offset + sf::Vector2f(0, side));
    }

    if (type.color == Color::White)
        return (offset + sf::Vector2f(static_cast<int>(type.type)*side, 7*side));
    else
        return (offset + sf::Vector2f(static_cast<int>(type.type)*side, 0));
}
