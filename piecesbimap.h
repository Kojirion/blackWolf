#ifndef PIECESBIMAP_H
#define PIECESBIMAP_H
#include <boost/bimap.hpp>
#include <boost/bimap/support/lambda.hpp>
#include "piecesprite.h"

class squareId{
public:
    int row;
    int col;

    squareId(int theRow, int theCol):
        row(theRow),col(theCol)
    {

    }

    bool operator< (const squareId &that) const{
        if(row==that.row) return (col<that.col);
        return (row<that.row);
    }
};

class piecesBimap
{
public:
    //friend class spriteHelper;

    typedef boost::bimap<squareId, pieceSprite> cellsNpieces;

    /*class piecePointer{
    private:
        cellsNpieces::right_iterator pointer;
    public:
        piecePointer():
            pointer(pieces.end())
        {

        }

        bool isValid(){
            return (pointer != pieces.end());
        }

        void invalidate(){
            poitner = pieces.end();
        }

    };*/

    class iterator{
    private:
        const piecesBimap *parent; //necessary?
        cellsNpieces::right_iterator pos;
    public:
        iterator(const piecesBimap *p, cellsNpieces::right_iterator thePos):
            parent(p), pos(thePos)
        {

        }

        bool operator!= (const iterator &other){
            return (pos != other.pos);
        }

        pieceSprite& operator*() const;

        const iterator& operator++(){
            ++pos;
            return *this;
        }
    };

    class spriteHelper{ //helps use the sprite
    private:
        piecesBimap &parent;
        const int rowId;
        const int colId;

        pieceSprite changePosition(pieceSprite piece, const sf::Vector2f position) const
        {
            piece.setPosition(position);
            return piece;
        }

    public:
        spriteHelper(piecesBimap &p, const int theRowId, const int theColId):
            parent(p), rowId(theRowId), colId(theColId)
        {

        }

        void moveTo(const int destRow, const int destCol, const sf::Vector2f position){
            cellsNpieces::right_iterator pieceToMove = parent.pieces.project_right(parent.pieces.left.find(squareId(rowId,colId)));
            parent.pieces.right.modify_data(pieceToMove, boost::bimaps::_data = squareId(destRow,destCol));
            parent.pieces.right.modify_key(pieceToMove, boost::bimaps::_key = changePosition(pieceToMove->first,position));
        }

        void erase(){
            squareId toDelete(rowId,colId);

            parent.pieces.left.erase(toDelete);
        }

        void insert(const pieceSprite &toAdd){
            squareId cellId(rowId,colId);
            parent.pieces.insert(cellsNpieces::value_type(cellId,toAdd));
        }


    };

    class indexHelper{ //helps get to the pieceSprite at given square
    private:
        piecesBimap &parent;
        int rowId;

    public:
        indexHelper(piecesBimap &p, int theRowId):
            parent(p), rowId(theRowId)
        {

        }

        spriteHelper operator[](const int colId)  const {
            return spriteHelper(parent, rowId, colId);
        }
    };

    indexHelper operator[](const int rowId){
        return indexHelper(*this, rowId);
    }

    iterator begin(){
        return iterator(this, pieces.right.begin());
    }

    iterator end(){
        return iterator(this, pieces.right.end());
    }

    piecesBimap();

private:

    cellsNpieces pieces;
};

#endif // PIECESBIMAP_H
