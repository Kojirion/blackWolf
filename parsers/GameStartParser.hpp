#pragma once
#include "Grammars.hpp"
#include <boost/fusion/include/vector.hpp>
#include "../messages/GameStart.hpp"

struct GameStartParser : grammar<Iterator, Messages::NewGameMessage()>
{
    GameStartParser();

    rule<Iterator> name, rating, rated_mode, time_mode;
    rule<Iterator, Messages::NewGameMessage()> start;
};
