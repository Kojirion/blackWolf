#include "Move.hpp"

std::ostream &operator<<(std::ostream &stream, const Move &move)
{
    stream << move.from << move.to;
    return stream;
}

bool operator==(const Move &lhs, const Move &rhs)
{
    return (lhs.from == rhs.from) && (lhs.to == rhs.to);
}
