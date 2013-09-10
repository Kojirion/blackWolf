#include "piecesbimap.h"
#include <boost/foreach.hpp>

void piecesBimap::clear()
{
    pieces.erase(pieces.begin(),pieces.end());
}

piecesBimap::piecesBimap()
{
}


const pieceSprite &piecesBimap::iterator::operator *() const
{
    return pos->first;
}
