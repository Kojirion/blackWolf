#include "SessionStartParser.hpp"
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

using namespace boost::spirit::qi;

SessionStartParser::SessionStartParser() : base_type(start){
    start = omit["**** Starting FICS session as "] >> +alnum >> omit[-lit("(U)") >> " ****"];
}
