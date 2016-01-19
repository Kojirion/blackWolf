#include <boost/test/unit_test.hpp>
#include "../parsers/GameEndParser.hpp"
#include <boost/spirit/include/qi.hpp>

using boost::spirit::qi::parse;

BOOST_AUTO_TEST_CASE(ParsingGameEnd)
{
    std::string given_1 = "{Game 83 (Kojijay vs. Thulius) Kojijay resigns} 0-1";
    std::string given_2 = "{Game 254 (Thulius vs. Kojijay) Game drawn by mutual agreement} 1/2-1/2";

    Color winner_1, winner_2;

    GameEndParser gameEndParser;

    auto success_1 = parse(given_1.begin(), given_1.end(), gameEndParser, winner_1);
    auto success_2 = parse(given_2.begin(), given_2.end(), gameEndParser, winner_2);

    BOOST_CHECK(success_1);
    BOOST_CHECK(success_2);
    BOOST_CHECK(winner_1==Color::Black);
    BOOST_CHECK(winner_2==Color::Both);
}
