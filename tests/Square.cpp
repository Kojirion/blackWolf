#include <boost/test/unit_test.hpp>
#include "../BlackWolf.hpp"
#include "../parsers/SquareParser.hpp"
#include <boost/spirit/include/qi.hpp>

using boost::spirit::qi::parse;

BOOST_AUTO_TEST_CASE(ParsingSquare)
{
    std::string given = "c4";

    Square square;

    SquareParser grammar;

    auto success = parse(given.begin(), given.end(), grammar, square);
    BOOST_CHECK(success);
    BOOST_CHECK((square == Square{3, 2}));

    std::string given_2 = "c4d5";

    boost::fusion::vector<Square, Square> squares;

    using boost::fusion::at_c;

    auto success_2 = parse(given_2.begin(), given_2.end(), grammar >> grammar, squares);
    BOOST_CHECK(success_2);
    Square square_1{3,2}, square_2{4,3};
    BOOST_CHECK_EQUAL(at_c<0>(squares), square_1);
    BOOST_CHECK_EQUAL(at_c<1>(squares), square_2);
}
