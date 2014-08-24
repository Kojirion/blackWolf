#include "GameStartParser.hpp"
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

using namespace boost::spirit::qi;

GameStartParser::GameStartParser() : base_type(start){

    name = +alnum;
    rating = '(' >> (uint_ | "++++") >> ')';
    rated_mode = +alnum;
    time_mode = +alnum;
    start = omit["Creating: "] >> as_string[+alnum] >> omit[' ' >> rating >> ' ']
                         >> as_string[+alnum] >> omit[' ' >> rating >> ' ' >> rated_mode >> ' ' >> time_mode >> ' '] >> int_ >> omit[' ' >> uint_];
}
