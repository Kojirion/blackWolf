#pragma once
#include "Square.hpp"

struct Move{
    Square from;
    Square to;
};

bool operator==(const Move& lhs, const Move& rhs);
std::ostream& operator<<(std::ostream& stream, const Move& move);
