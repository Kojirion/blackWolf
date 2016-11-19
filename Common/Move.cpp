#include "Move.hpp"

std::ostream &operator<<(std::ostream &stream, const Move &move)
{
    stream << move.square_1 << move.square_2;
    return stream;
}

bool operator==(const Move &lhs, const Move &rhs)
{
    return (lhs.square_1 == rhs.square_1) && (lhs.square_2 == rhs.square_2);
}
