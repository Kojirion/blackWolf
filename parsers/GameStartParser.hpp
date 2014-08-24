#pragma once
#include "Grammars.hpp"
#include <boost/fusion/include/vector.hpp>

using GameStartTuple = boost::fusion::vector<std::string, std::string, int>;

struct GameStartParser : grammar<Iterator, GameStartTuple()>
{
    GameStartParser();

    rule<Iterator> name, rating, rated_mode, time_mode;
    rule<Iterator, GameStartTuple()> start;
};
