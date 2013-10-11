#include "blackWolf.h"
#include <boost/assert.hpp>

//bw operator-(const bw rhs)
//{
//    //could edit to make this usable by pieces as well
//    if (rhs==bw::White) return bw::Black;
//    if (rhs==bw::Black) return bw::White;
//    return rhs;
////    if (rhs==bw::None) return bw::None;
////    if (rhs==(bw::White | bw::Black)) return (bw::White | bw::Black);
//}

//constexpr bw operator&(const bw lhs, const bw rhs)
//{
//    return static_cast<bw>(static_cast<int>(lhs) & static_cast<int>(rhs));
//}

///*bool operator==(const bw lhs, const bw rhs)
//{
//    return (lhs == rhs);
//}*/

//constexpr bool check(const bw toCheck)
//{
//    return (toCheck != bw::None);
//}


//constexpr bw operator |(const bw lhs, const bw rhs)
//{
//    return static_cast<bw>(static_cast<int>(lhs) | static_cast<int>(rhs));
//}


//int bwToInt(bw piece)
//{
//    switch (piece) {
//    case bw::Queen: return 4;
//    case bw::Bishop: return 2;
//    case bw::Knight: return 3;
//    case bw::Rook: return 1;
//    default: return -1; //appease compiler
//    }
//}


//bw intToBw(int piece)
//{
//    switch (piece) {
//    case 4: return bw::Queen;
//    case 2: return bw::Bishop;
//    case 3: return bw::Knight;
//    case 1: return bw::Rook;
//    default: return bw::None; //appease compiler
//    }
//}


int sign(Color color)
{
    if (color == Color::White) return 1;
    BOOST_ASSERT_MSG(color == Color::Black, "Invalid color sign request");
    return -1;
}


Color operator !(Color rhs)
{
    if (rhs==Color::White) return Color::Black;
    if (rhs==Color::Black) return Color::White;
    return rhs;
}


bool operator ==(const Unit &lhs, const Unit &rhs)
{
    return (lhs.color == rhs.color) && (lhs.type == rhs.type);
}


bool operator &(const Color &lhs, const Color &rhs)
{
    return static_cast<int>(lhs) & static_cast<int>(rhs);
}
