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
    Shadow = 1 << 6,
    White  = 1 << 7,
    Black  = 1 << 8
};

bw operator-(const bw rhs);

bw operator&(const bw lhs, const bw rhs);

bw operator|(const bw lhs, const bw rhs);

bool check(const bw toCheck);

//for compatibility until getting rid of all the ints
int bwToInt(bw piece);
bw intToBw(int piece);

#endif // BLACKWOLF_H
