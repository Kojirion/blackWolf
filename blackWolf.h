#ifndef BLACKWOLF_H
#define BLACKWOLF_H
#include <boost/assert.hpp>

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

bw operator-(const bw rhs)
{
    if (rhs==bw::White) return bw::Black;
    if (rhs==bw::Black) return bw::White;
    return rhs;
//    if (rhs==bw::None) return bw::None;
//    if (rhs==(bw::White | bw::Black)) return (bw::White | bw::Black);
}

bw operator&(const bw lhs, const bw rhs)
{
    return (lhs & rhs);
}

/*bool operator==(const bw lhs, const bw rhs)
{
    return (lhs == rhs);
}*/

bool check(const bw toCheck)
{
    return (toCheck != bw::None);
}

#endif // BLACKWOLF_H
