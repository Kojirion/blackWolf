#pragma once
#include "Grammars.hpp"
#include <boost/spirit/include/qi_symbols.hpp>

class Square;

using boost::spirit::qi::symbols;

struct ColumnMap : symbols<char, int>
{
    ColumnMap();
};



struct SquareParser : grammar<Iterator, Square()> {
    SquareParser();

    rule<Iterator, Square()> start;
};
