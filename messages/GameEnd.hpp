#pragma once
#include "Message.hpp"
#include "../BlackWolf.hpp"
#include <boost/fusion/include/adapt_struct.hpp>

namespace Messages{

struct GameEnd : public Message {
    GameEnd();

    Color result;
};

}

BOOST_FUSION_ADAPT_STRUCT(
        Messages::GameEnd,
        (Color, result)
        )
