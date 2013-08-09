#include "piecesbimap.h"

piecesBimap::piecesBimap()
{
}


pieceSprite &piecesBimap::iterator::operator *() const
{
    return pos.first;
}
