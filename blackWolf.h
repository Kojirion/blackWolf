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
    if (rhs==bw::None) return bw::None;
    if (rhs==bw::White) return bw::Black;
    BOOST_ASSERT_MSG(rhs==bw::Black, "Trying to flip piece enum");
    return bw::White;
}

#endif // BLACKWOLF_H
