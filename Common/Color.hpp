#pragma once

enum class Color{
    None = 0,
    White = 1 << 0,
    Black = 1 << 1,
    Both = White | Black
};

bool operator &(const Color& lhs, const Color& rhs);
Color operator!(Color rhs);
int sign(Color color);
