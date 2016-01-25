#include <boost/test/unit_test.hpp>
#include "../parsers/GameStartParser.hpp"
#include <boost/spirit/include/qi.hpp>

using boost::spirit::qi::parse;

BOOST_AUTO_TEST_CASE(ParsingGameStart)
{
    std::string given = "Creating: Kojijay (++++) Thulius (1690) unrated blitz 2 12";

    GameStartParser gameStartParser;
    NewGameMessage newGameMessage;

    auto success = parse(given.begin(), given.end(), gameStartParser, newGameMessage);
    BOOST_CHECK(success);
    BOOST_CHECK_EQUAL(newGameMessage.p1, "Kojijay");
    BOOST_CHECK_EQUAL(newGameMessage.p2, "Thulius");
    BOOST_CHECK_EQUAL(newGameMessage.time, 2);
}
