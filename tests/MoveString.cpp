#include <boost/test/unit_test.hpp>
#include "../BlackWolf.hpp"
#include <string>
#include <boost/spirit/include/karma.hpp>

using boost::spirit::karma::generate;

BOOST_AUTO_TEST_CASE(GeneratingMoveString)
{
    Move given{{0,0},{5,0}};
    std::string expected("a1a6");

    MoveStringGrammar moveStringGrammar;

    std::string generated;

    generate(generated.begin(), moveStringGrammar, position.m_cells);

    BOOST_CHECK_EQUAL(generated, expected);
}
