#include "SquareParser.hpp"
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include "../BlackWolf.hpp"

using namespace boost::spirit::qi;

ColumnMap::ColumnMap(){
    add
        ("a", 0)
        ("b", 1)
        ("c", 2)
        ("d", 3)
        ("e", 4)
        ("f", 5)
        ("g", 6)
        ("h", 7)
    ;
}

ColumnMap columnMap;

SquareParser::SquareParser() : base_type(start)
{
    start = columnMap[boost::phoenix::bind(&Square::col, _val) = boost::spirit::_1] >> int_[boost::phoenix::bind(&Square::row, _val) = boost::spirit::_1 - 1];
}
