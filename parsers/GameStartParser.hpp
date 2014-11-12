#pragma once
#include "Grammars.hpp"
#include <boost/fusion/include/vector.hpp>
#include "../Entity.hpp"

struct GameStartParser : grammar<Iterator, NewGameMessage()>
{
    GameStartParser();

    rule<Iterator> name, rating, rated_mode, time_mode;
    rule<Iterator, NewGameMessage()> start;
};
