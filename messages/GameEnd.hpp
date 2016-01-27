#pragma once
#include "Message.hpp"
#include "../BlackWolf.hpp"
#include <boost/fusion/include/adapt_struct.hpp>

namespace Messages{

struct EndGameMessage : public Message {
    EndGameMessage();

    Color result;
};

}

BOOST_FUSION_ADAPT_STRUCT(
        Messages::EndGameMessage,
        (Color, result)
        )
