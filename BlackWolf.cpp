#include "BlackWolf.hpp"
#include <boost/assert.hpp>

int sign(Color color)
{
    if (color == Color::White) return 1;
    BOOST_ASSERT_MSG(color == Color::Black, "Invalid color sign request");
    return -1;
}


Color operator !(Color rhs)
{
    if (rhs==Color::White) return Color::Black;
    if (rhs==Color::Black) return Color::White;
    BOOST_ASSERT_MSG(true, "Requesting to flip none or both");
    return rhs;
}


bool operator ==(const Unit &lhs, const Unit &rhs)
{
    return (lhs.color == rhs.color) && (lhs.piece == rhs.piece);
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

sf::Vector2f typeToTexPos(const Unit &type)
{
    BOOST_ASSERT_MSG((type.color==Color::Black)||(type.color==Color::White), "Invalid piece color");
    BOOST_ASSERT_MSG((type.piece!=Piece::None)&&(type.piece!=Piece::Shadow), "Invalid piece type");


    const sf::Vector2f offset(20.f, 20.f);
    const int side = 50;

    if (type.piece == Piece::Pawn){
        if (type.color == Color::White)
            return (offset + sf::Vector2f(0, 6*side));
        else return (offset + sf::Vector2f(0, side));
    }

    if (type.color == Color::White)
        return (offset + sf::Vector2f(static_cast<int>(type.piece)*side, 7*side));
    else
        return (offset + sf::Vector2f(static_cast<int>(type.piece)*side, 0));
}
