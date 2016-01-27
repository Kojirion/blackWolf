#pragma once
#include "Message.hpp"
#include "../BlackWolf.hpp"
#include <boost/fusion/include/adapt_struct.hpp>

namespace Messages{

struct GameStart : public Message {
    GameStart();

    std::string p1, p2;
    int time;
    Color user;
};

}

BOOST_FUSION_ADAPT_STRUCT(
        Messages::GameStart,
        (std::string, p1)
        (std::string, p2)
        (int, time)
        )
