#ifndef PIECESBIMAP_H
#define PIECESBIMAP_H
#include <boost/bimap.hpp>
#include <boost/bimap/support/lambda.hpp>
#include "../../views/components/piecesprite.h"

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


    typedef boost::bimap<squareId, pieceSprite> cellsNpieces;


    class iterator{
    private:
        piecesBimap *parent;
        cellsNpieces::right_iterator pos;
    public:
        iterator(piecesBimap *p, cellsNpieces::right_iterator thePos):
            parent(p), pos(thePos)
        {

        }

        iterator(piecesBimap *p):
            parent(p), pos(p->pieces.right.end())
        {

        }


        bool operator!= (const iterator &other){
            return (pos != other.pos);
        }

        const pieceSprite& operator*() const;

        const iterator& operator++(){
            ++pos;
            return *this;
        }

        bool isValid(){
            return (pos != parent->pieces.right.end());
        }

        void invalidate(){
            pos = parent->pieces.right.end();
        }

        int getRow()
        {
            return pos->second.row;
        }

        int getCol()
        {
            return pos->second.col;
        }

        cellsNpieces::right_iterator getRI()
        {
            return pos;
        }
    };

    class spriteHelper{ //helps use the sprite
    private:
        piecesBimap &parent;
        int rowId;
        int colId;
        cellsNpieces::right_iterator pieceToUse;

        pieceSprite changePosition(pieceSprite piece, const sf::Vector2f& position) const
        {
            piece.setPosition(position);
            return piece;
        }

        pieceSprite changeTexture(pieceSprite piece, const sf::Texture& toSet) const
        {
            piece.setTexture(toSet);
            return piece;
        }

    public:
        spriteHelper(piecesBimap &p, const int theRowId, const int theColId):
            parent(p), rowId(theRowId), colId(theColId)
        {
            pieceToUse = parent.pieces.project_right(parent.pieces.left.find(squareId(rowId,colId)));
        }

        spriteHelper(piecesBimap &p, const pieceSprite& piece):
            parent(p)
        {
            pieceToUse = parent.pieces.right.find(piece);
            rowId = pieceToUse->second.row;
            colId = pieceToUse->second.col;
        }

        spriteHelper(piecesBimap &p, iterator piece):
            parent(p),
            rowId(piece.getRow()),
            colId(piece.getCol()),
            pieceToUse(piece.getRI())
        {


        }

        void sendTo(const sf::Vector2f position){
            parent.pieces.right.modify_key(pieceToUse, boost::bimaps::_key = changePosition(pieceToUse->first,position));
        }

        void moveTo(const int destRow, const int destCol, const sf::Vector2f position){
            parent.pieces.right.modify_data(pieceToUse, boost::bimaps::_data = squareId(destRow,destCol));
            parent.pieces.right.modify_key(pieceToUse, boost::bimaps::_key = changePosition(pieceToUse->first,position));
        }

        sf::Vector2f getPosition()
        {
            return pieceToUse->first.getPosition();
        }

        bool erase(){ //returns true if element erased
            squareId toDelete(rowId,colId);

            return (parent.pieces.left.erase(toDelete));
        }

        void insert(const pieceSprite &toAdd){
            squareId cellId(rowId,colId);
            parent.pieces.insert(cellsNpieces::value_type(cellId,toAdd));
        }

        bw getSide() const
        {
            return pieceToUse->first.getSide();
        }

        int getRow() const
        {
            return rowId;
        }

        int getCol() const
        {
            return colId;
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

    spriteHelper operator[](const pieceSprite &toUse){
        return spriteHelper(*this, toUse);
    }

    spriteHelper operator[](iterator toUse){
        return spriteHelper(*this, toUse);
    }

    iterator begin(){
        return iterator(this, pieces.right.begin());
    }

    iterator end(){
        return iterator(this, pieces.right.end());
    }

    iterator spriteToIt(const pieceSprite &givenSprite)
    {
        return iterator(this, pieces.right.find(givenSprite));
    }

    void clear();

    void erase(const pieceSprite& sprite)
    {
        pieces.right.erase(sprite);
    }


    piecesBimap();

private:

    cellsNpieces pieces;
};

#endif // PIECESBIMAP_H
