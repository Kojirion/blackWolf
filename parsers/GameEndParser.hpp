#pragma once
#include "Grammars.hpp"
#include "../BlackWolf.hpp"
#include <boost/spirit/include/qi_symbols.hpp>

using boost::spirit::qi::symbols;

struct ResultMap : symbols<char, Color>
{
    ResultMap();
};

struct GameEndParser : grammar<Iterator, Color()>
{
    GameEndParser();

    rule<Iterator> name;
    rule<Iterator, Color()> start;
};
