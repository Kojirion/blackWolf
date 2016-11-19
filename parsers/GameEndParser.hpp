#pragma once
#include "Grammars.hpp"
#include <boost/spirit/include/qi_symbols.hpp>
#include "../messages/GameEnd.hpp"

using boost::spirit::qi::symbols;

struct ResultMap : symbols<char, Color>
{
    ResultMap();
};

struct GameEndParser : grammar<Iterator, Messages::GameEnd()>
{
    GameEndParser();

    rule<Iterator> name;
    rule<Iterator, Messages::GameEnd()> start;
};
