#ifndef BLACKWOLF_H
#define BLACKWOLF_H

enum class bw
{
    None   = 0,
    Pawn   = 1 << 0,
    Rook   = 1 << 1,
    Knight = 1 << 2,
    Bishop = 1 << 3,
    Queen  = 1 << 4,
    King   = 1 << 5,
    White  = 1 << 6,
    Black  = 1 << 7
};

bw operator-(const bw rhs);

bw operator&(const bw lhs, const bw rhs);

bw operator|(const bw lhs, const bw rhs);

bool check(const bw toCheck);

#endif // BLACKWOLF_H
