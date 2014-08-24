#include "GameEndParser.hpp"
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

using namespace boost::spirit::qi;

ResultMap::ResultMap(){
    add
        ("1-0", Color::White)
        ("1/2-1/2", Color::Both)
        ("0-1", Color::Black)
    ;
}

ResultMap resultMap;

GameEndParser::GameEndParser(): base_type(start)
{
    name = +alnum;
    //result = (lit("1-0") >> attr(Color::White)) | (lit("1/2-1/2") >> attr(Color::Both)) | (lit("0-1") >> Color::Black);
    start = omit["{Game " >> uint_ >> " (" >> name >> " vs. " >> name >> ") " >> +(alnum | ' ') >> "} "] >> resultMap;
}
