#pragma once
#include "Message.hpp"
#include "../BlackWolf.hpp"
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/optional.hpp>

using ParsedPosition = std::vector<std::vector<Piece>>;

namespace Messages{

struct GameState : public Message {
    GameState();
    ParsedPosition position;
    Color turnColor;
    int white_time, black_time;
    boost::optional<Square> target_square;
    std::string move;
};

}

BOOST_FUSION_ADAPT_STRUCT(
        Messages::GameState,
        (ParsedPosition, position)
        (Color, turnColor)
        (int, white_time)
        (int, black_time)
        (boost::optional<Square>, target_square)
        (std::string, move)
        )
