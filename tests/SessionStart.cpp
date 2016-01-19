#include <boost/test/unit_test.hpp>
#include "../parsers/SessionStartParser.hpp"
#include <boost/spirit/include/qi.hpp>

using boost::spirit::qi::parse;

BOOST_AUTO_TEST_CASE(ParsingSessionStart)
{
    std::string given_1 = "**** Starting FICS session as Kojijay(U) ****";
    std::string given_2 = "**** Starting FICS session as Thulius ****";

    SessionStartParser sessionStartParser;

    auto success_1 = parse(given_1.begin(), given_1.end(), sessionStartParser);
    auto success_2 = parse(given_2.begin(), given_2.end(), sessionStartParser);
    BOOST_CHECK(success_1);
    BOOST_CHECK(success_2);
}
