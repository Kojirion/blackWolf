#pragma once
#include "Grammars.hpp"

struct SessionStartParser : grammar<Iterator, std::string()>
{
    SessionStartParser();

    rule<Iterator, std::string()> start;
};
