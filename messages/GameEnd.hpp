#pragma once
#include "Message.hpp"
#include <boost/fusion/include/adapt_struct.hpp>
#include "../Common/Color.hpp"

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
