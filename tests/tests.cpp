#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <boost/test/included/unit_test.hpp>
#include "../parsers/GameStateParser.hpp"
#include "../parsers/SessionStartParser.hpp"
#include "../parsers/GameStartParser.hpp"
#include "../parsers/GameEndParser.hpp"
#include <boost/fusion/include/vector.hpp>
#include <boost/spirit/include/qi.hpp>

using boost::spirit::qi::parse;

BOOST_AUTO_TEST_CASE(parsing_square)
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


BOOST_AUTO_TEST_CASE(parsing_game_state)
{
    GameStateParser my_grammar;

    GameStateTuple gameState;

    std::string given("<12> rnbqk--r ppp--ppp -----n-- ---p--N- --P-p--- B-P---P- P--PPP-P R--QKB-R B -1 1 1 1 1 1 197 chessbadboy SevenChecks 0 60 0 36 36 3511 3086 7 B/c1-a3 (0:45) Ba3 0 1 0");

    auto success = parse(given.begin(), given.end(), my_grammar, gameState);

    using boost::fusion::at_c;

    BOOST_CHECK(success);
    Square square_1{0,2}, square_2{2,0};

    BOOST_CHECK_EQUAL(at_c<2>(gameState), square_1);
    BOOST_CHECK_EQUAL(at_c<3>(gameState), square_2);
}



BOOST_AUTO_TEST_CASE(parsing_game_creation)
{
    std::string given = "Creating: Kojijay (++++) Thulius (1690) unrated blitz 2 12";

    auto success = parse(given.begin(), given.end(), GameStartParser());
    BOOST_CHECK(success);
}



BOOST_AUTO_TEST_CASE(parsing_game_end)
{
    std::string given_1 = "{Game 83 (Kojijay vs. Thulius) Kojijay resigns} 0-1";
    std::string given_2 = "{Game 254 (Thulius vs. Kojijay) Game drawn by mutual agreement} 1/2-1/2";

    auto success_1 = parse(given_1.begin(), given_1.end(), GameEndParser());
    auto success_2 = parse(given_2.begin(), given_2.end(), GameEndParser());
    BOOST_CHECK(success_1);
    BOOST_CHECK(success_2);
}



BOOST_AUTO_TEST_CASE(parsing_session_start)
{
    std::string given_1 = "**** Starting FICS session as Kojijay(U) ****";
    std::string given_2 = "**** Starting FICS session as Thulius ****";

    auto success_1 = parse(given_1.begin(), given_1.end(), SessionStartParser());
    auto success_2 = parse(given_2.begin(), given_2.end(), SessionStartParser());
    BOOST_CHECK(success_1);
    BOOST_CHECK(success_2);
}
