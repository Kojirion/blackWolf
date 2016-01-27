#pragma once
#include "Message.hpp"
#include "../BlackWolf.hpp"
#include <boost/fusion/include/adapt_struct.hpp>

struct EndGameMessage : public Message {
    EndGameMessage();

    Color result;
};

BOOST_FUSION_ADAPT_STRUCT(
        EndGameMessage,
        (Color, result)
        )
