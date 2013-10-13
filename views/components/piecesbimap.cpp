#include "piecesbimap.h"

piecesBimap::piecesBimap()
{

}

piecesBimap::iterator piecesBimap::begin()
{
    return pieces.right.begin();
}

piecesBimap::iterator piecesBimap::end()
{
    return pieces.right.end();
}

void piecesBimap::makeMove(const Move &move)
{
    pieces.by<squareId>().modify_key(pieces.by<squareId>().find(move.square_1), boost::bimaps::_key = move.square_2);
}

void piecesBimap::clear()
{
    pieces.erase(pieces.begin(),pieces.end());
}

bool piecesBimap::erase(const Square &square)
{
    return pieces.left.erase(square);
}

const pieceSprite &piecesBimap::operator [](const Square &square) const
{
    return pieces.by<squareId>().find(square)->get<pieceId>();
}

const pieceSprite &piecesBimap::operator [](const pieceSprite &piece) const
{
    return pieces.by<pieceId>().find(piece)->get<pieceId>();
}


