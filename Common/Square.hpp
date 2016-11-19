#pragma once
#include <ostream>

struct Square{
    int row;
    int col;
};


bool operator<(const Square& lhs, const Square& rhs);
bool operator==(const Square& lhs, const Square& rhs);

std::ostream& operator<<(std::ostream& stream, const Square& square);
