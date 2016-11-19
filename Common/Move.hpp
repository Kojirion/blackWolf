#pragma once
#include "Square.hpp"

struct Move{
    Square square_1;
    Square square_2;
};

bool operator==(const Move& lhs, const Move& rhs);
std::ostream& operator<<(std::ostream& stream, const Move& move);
