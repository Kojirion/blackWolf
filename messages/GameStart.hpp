#pragma once
#include "Message.hpp"
#include "../BlackWolf.hpp"
#include <boost/fusion/include/adapt_struct.hpp>

namespace Messages{

struct NewGameMessage : public Message {
    NewGameMessage();

    std::string p1, p2;
    int time;
    Color user;
};

}

BOOST_FUSION_ADAPT_STRUCT(
        Messages::NewGameMessage,
        (std::string, p1)
        (std::string, p2)
        (int, time)
        )
