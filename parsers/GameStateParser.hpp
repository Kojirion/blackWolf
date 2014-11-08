#pragma once
#include "Grammars.hpp"
#include "SquareParser.hpp"
#include "../BlackWolf.hpp"
#include <boost/fusion/include/vector.hpp>
#include "../Entity.hpp"

//gameStateParser.move_result, gameStateParser.white_time_result, gameStateParser.black_time_result, Piece::None

//using ParsedPosition = std::vector<std::vector<Unit>>;

//using GameStateTuple = boost::fusion::vector<ParsedPosition, int, int, std::string>;

struct GameStateParser : grammar<Iterator, GameStateMessage()>
{
    GameStateParser();

    rule<Iterator> double_advance, castling_right, irreversible_moves, game_id, name, relation,
    time_taken, move;
    rule<Iterator, std::string()> pretty_move;
    rule<Iterator, std::vector<Unit>()> row;
    rule<Iterator, GameStateMessage()> start;
    //rule<Iterator, boost::fusion::vector<Square, Square, boost::optional<Piece>>()> verbose_move;
    SquareParser square; //can this be moved into source?
};
