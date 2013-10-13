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

//    class spriteHelper{ //helps use the sprite
//    private:
//        piecesBimap &parent;
//        int rowId;
//        int colId;
//        SquaresToPieces::right_iterator pieceToUse;

//        pieceSprite changePosition(pieceSprite piece, const sf::Vector2f& position) const
//        {
//            piece.setPosition(position);
//            return piece;
//        }

//        pieceSprite changeTexture(pieceSprite piece, const sf::Texture& toSet) const
//        {
//            //piece.setTexture(toSet);
//            return piece;
//        }

//    public:
//        spriteHelper(piecesBimap &p, const int theRowId, const int theColId):
//            parent(p), rowId(theRowId), colId(theColId)
//        {
//            pieceToUse = parent.pieces.project_right(parent.pieces.left.find(Square{rowId,colId}));
//        }

//        spriteHelper(piecesBimap &p, const pieceSprite& piece):
//            parent(p)
//        {
//            pieceToUse = parent.pieces.right.find(piece);
//            rowId = pieceToUse->second.row;
//            colId = pieceToUse->second.col;
//        }

//        spriteHelper(piecesBimap &p, iterator piece):
//            parent(p),
//            rowId(piece.getRow()),
//            colId(piece.getCol()),
//            pieceToUse(piece.getRI())
//        {


//        }

//        void sendTo(const sf::Vector2f position){
//            parent.pieces.right.modify_key(pieceToUse, boost::bimaps::_key = changePosition(pieceToUse->first,position));
//        }

//        void moveTo(const int destRow, const int destCol, const sf::Vector2f position){
//            parent.pieces.right.modify_data(pieceToUse, boost::bimaps::_data = Square{destRow,destCol});
//            parent.pieces.right.modify_key(pieceToUse, boost::bimaps::_key = changePosition(pieceToUse->first,position));
//        }

//        sf::Vector2f getPosition()
//        {
//            return pieceToUse->first.getPosition();
//        }

//        bool erase(){ //returns true if element erased
//            Square toDelete{rowId,colId};

//            return (parent.pieces.left.erase(toDelete));
//        }

//        void insert(const pieceSprite &toAdd){
//            Square cellId{rowId,colId};
//            parent.pieces.insert(SquaresToPieces::value_type(cellId,toAdd));
//        }

//        Color getColor() const
//        {
//            return pieceToUse->first.getColor();
//        }

//        int getRow() const
//        {
//            return rowId;
//        }

//        int getCol() const
//        {
//            return colId;
//        }


//    };

//    class indexHelper{ //helps get to the pieceSprite at given square
//    private:
//        piecesBimap &parent;
//        int rowId;

//    public:
//        indexHelper(piecesBimap &p, int theRowId):
//            parent(p), rowId(theRowId)
//        {

//        }

//        spriteHelper operator[](const int colId)  const {
//            return spriteHelper(parent, rowId, colId);
//        }
//    };

//    indexHelper operator[](const int rowId){
//        return indexHelper(*this, rowId);
//    }

//    spriteHelper operator[](const pieceSprite &toUse){
//        return spriteHelper(*this, toUse);
//    }

//    spriteHelper operator[](iterator toUse){
//        return spriteHelper(*this, toUse);
//    }

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
