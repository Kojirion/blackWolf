#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>
#include "../BlackWolf.hpp"

BOOST_AUTO_TEST_CASE(parsing)
{
    Move move{{0,0}, {1,1}};
    Move move_2 = move;

    BOOST_CHECK(true);
}
