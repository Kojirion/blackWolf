#include <boost/test/unit_test.hpp>
#include <string>
#include "../generators/MoveStringGrammar.hpp"
#include <boost/spirit/include/karma.hpp>

using boost::spirit::karma::generate;

BOOST_AUTO_TEST_CASE(GeneratingMoveString)
{
    Move given{{0,0},{5,0}};
    std::string expected("a1a6");

    using Iterator = std::back_insert_iterator<std::string>;

    MoveStringGrammar<Iterator> moveStringGrammar;

    std::string generated;

    generate(Iterator(generated), moveStringGrammar, given);

    BOOST_CHECK_EQUAL(generated, expected);
}
