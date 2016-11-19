#pragma once
#include <boost/spirit/include/karma.hpp>

template <typename OutputIterator>
struct MoveStringGrammar : boost::spirit::karma::grammar<OutputIterator, Move()>
{
    MoveStringGrammar() : MoveStringGrammar::base_type(start, "MoveStringGrammar")
    {
        using namespace boost::spirit::karma;

        start = stream;
    }

    boost::spirit::karma::rule<OutputIterator, Move()> start;
};
