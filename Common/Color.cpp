#include "Color.hpp"
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

bool operator &(const Color &lhs, const Color &rhs)
{
    return static_cast<int>(lhs) & static_cast<int>(rhs);
}
