#pragma once
#include "Grammars.hpp"
#include "SquareParser.hpp"
#include "../BlackWolf.hpp"
#include <boost/fusion/include/vector.hpp>

//gameStateParser.move_result, gameStateParser.white_time_result, gameStateParser.black_time_result, Piece::None

using GameStateTuple = boost::fusion::vector<int, int, Square, Square/*, boost::optional<Piece>*/>;

struct GameStateParser : grammar<Iterator, GameStateTuple()>
{
    GameStateParser();

    rule<Iterator> piece, row, color, double_advance, castling_right, irreversible_moves, game_id, name, relation,
    time_taken, pretty_move, move;
    rule<Iterator, GameStateTuple()> start;
    //rule<Iterator, boost::fusion::vector<Square, Square, boost::optional<Piece>>()> verbose_move;
    SquareParser square; //can this be moved into source?
};
