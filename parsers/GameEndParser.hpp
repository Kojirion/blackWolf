#pragma once
#include "Grammars.hpp"
#include "../BlackWolf.hpp"
#include <boost/spirit/include/qi_symbols.hpp>
#include "../messages/GameEnd.hpp"

using boost::spirit::qi::symbols;

struct ResultMap : symbols<char, Color>
{
    ResultMap();
};

struct GameEndParser : grammar<Iterator, Messages::EndGameMessage()>
{
    GameEndParser();

    rule<Iterator> name;
    rule<Iterator, Messages::EndGameMessage()> start;
};
