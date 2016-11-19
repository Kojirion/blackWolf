#include "Square.hpp"

bool operator <(const Square &lhs, const Square &rhs)
{
    if(lhs.row==rhs.row)
        return (lhs.col<rhs.col);
    return (lhs.row<rhs.row);
}

std::ostream &operator<<(std::ostream &stream, const Square &square)
{
    static const char* colToChar = "abcdefgh";
    stream << colToChar[square.col] << (square.row+1);
    return stream;
}

bool operator==(const Square &lhs, const Square &rhs)
{
    return (lhs.row == rhs.row) && (lhs.col == rhs.col);
}
