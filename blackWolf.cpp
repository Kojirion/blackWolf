#include "blackWolf.h"
//#include <boost/assert.hpp>

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
    return static_cast<bw>(static_cast<int>(lhs) & static_cast<int>(rhs));
}

/*bool operator==(const bw lhs, const bw rhs)
{
    return (lhs == rhs);
}*/

bool check(const bw toCheck)
{
    return (toCheck != bw::None);
}


bw operator |(const bw lhs, const bw rhs)
{
    return static_cast<bw>(static_cast<int>(lhs) | static_cast<int>(rhs));
}


int bwToInt(bw piece)
{
    switch (piece) {
    case bw::Queen: return 4;
    case bw::Bishop: return 2;
    case bw::Knight: return 3;
    case bw::Rook: return 1;
    default: return -1; //appease compiler
    }
}


bw intToBw(int piece)
{
    switch (piece) {
    case 4: return bw::Queen;
    case 2: return bw::Bishop;
    case 3: return bw::Knight;
    case 1: return bw::Rook;
    default: return bw::None; //appease compiler
    }
}
