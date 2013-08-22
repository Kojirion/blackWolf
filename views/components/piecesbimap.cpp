#include "piecesbimap.h"

void piecesBimap::clear()
{
    for (auto &piece : pieces){
        pieces.left.erase(piece.left);
    }
}

piecesBimap::piecesBimap()
{
}


const pieceSprite &piecesBimap::iterator::operator *() const
{
    return pos->first;
}
