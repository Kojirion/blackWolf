#ifndef PIECESBIMAP_H
#define PIECESBIMAP_H
#include <boost/bimap.hpp>
#include <boost/bimap/support/lambda.hpp>
#include "../../views/components/piecesprite.h"

class piecesBimap
{
public:

    struct squareId  {};
    struct pieceId {};

    typedef boost::bimap<boost::bimaps::tagged<Square, squareId>, boost::bimaps::tagged<pieceSprite, pieceId> > SquaresToPieces;

    typedef SquaresToPieces::right_iterator iterator;

    iterator begin();

    iterator end();

    iterator spriteToIt(const pieceSprite &givenSprite)
    {
        return pieces.right.find(givenSprite);
    }

    void makeMove(const Move& move);

    void clear();

    void erase(const pieceSprite& piece)
    {
        pieces.right.erase(piece);
    }

    bool erase(const Square& square);

    const pieceSprite& operator[](const Square& square) const;

    const pieceSprite& operator[](const pieceSprite& piece) const;

    piecesBimap();

private:

    SquaresToPieces pieces;
};

#endif // PIECESBIMAP_H
