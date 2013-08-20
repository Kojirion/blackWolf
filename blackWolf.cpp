#include "blackWolf.h"

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
