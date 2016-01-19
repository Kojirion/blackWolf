#include <boost/test/unit_test.hpp>
#include "../parsers/GameStateParser.hpp"
#include <boost/spirit/include/qi.hpp>

using boost::spirit::qi::parse;

BOOST_AUTO_TEST_CASE(ParsingGameState)
{
    GameStateParser my_grammar;

    GameStateMessage gameState;

    std::string given("<12> rnbqk--r ppp--ppp -----n-- ---p--N- --P-p--- B-P---P- P--PPP-P R--QKB-R B -1 1 1 1 1 1 197 chessbadboy SevenChecks 0 60 0 36 36 3511 3086 7 B/c1-a3 (0:45) Ba3 0 1 0");

    auto success = parse(given.begin(), given.end(), my_grammar, gameState);

    std::string given_2("<12> rnbqkbnr pppppppp -------- -------- -------- -------- PPPPPPPP RNBQKBNR W -1 1 1 1 1 0 299 Thulius Kojijay -1 2 12 39 39 120 120 1 none (0:00) none 1 0 0");

    auto success_2 = parse(given_2.begin(), given_2.end(), my_grammar);

    std::string given_3("<12> rnbqkbnr ppp-pppp -------- -B-p---- ----P--- -------- PPPP-PPP RNBQK-NR B -1 1 1 1 1 1 193 Kojijay Thulius -1 2 12 39 39 128 120 2 B/f1-b5 (0:04) Bb5+ 0 1 0");

    auto success_3 = parse(given_3.begin(), given_3.end(), my_grammar);

    std::string given_4("<12> rnbqk--r ppppbppp -----n-- ----p--- --B-P--- -----N-- PPPP-PPP RNBQ-RK- B -1 0 0 1 1 5 190 Thulius Kojijay 1 2 12 39 39 115 134 4 o-o (0:35) O-O 1 1 2940");
    std::string given_5("<12> --kr---r pppqbppp --npbn-- ----p--- --B-P--- -PN--N-- PBPPQPPP R----RK- W -1 0 0 0 0 3 190 Thulius Kojijay -1 2 12 39 39 150 91 9 o-o (0:04) O-O-O 1 1 0");

    auto success_4 = parse(given_4.begin(), given_4.end(), my_grammar);
    auto success_5 = parse(given_5.begin(), given_5.end(), my_grammar);


    BOOST_CHECK(success);
    BOOST_CHECK(success_2);
    BOOST_CHECK(success_3);
    BOOST_CHECK(success_4);
    BOOST_CHECK(success_5);
}
