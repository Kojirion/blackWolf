#include "piecesbimap.h"

piecesBimap::piecesBimap()
{
}


const pieceSprite &piecesBimap::iterator::operator *() const
{
    return pos->first;
}
