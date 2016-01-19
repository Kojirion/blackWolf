#include <boost/test/unit_test.hpp>
#include "../parsers/GameStartParser.hpp"
#include <boost/spirit/include/qi.hpp>

using boost::spirit::qi::parse;

BOOST_AUTO_TEST_CASE(ParsingGameStart)
{
    std::string given = "Creating: Kojijay (++++) Thulius (1690) unrated blitz 2 12";

    auto success = parse(given.begin(), given.end(), GameStartParser());
    BOOST_CHECK(success);
}
