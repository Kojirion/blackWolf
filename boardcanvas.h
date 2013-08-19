//the board "view"
//a class built around sfg::Canvas

#ifndef BOARDCANVAS_H
#define BOARDCANVAS_H
#include <SFGUI/Canvas.hpp>
#include "piecesbimap.h"
#include "resourcemanager.h"
#include "completemove.h"

class boardCanvas
{
private:
    sfg::Canvas::Ptr window;
    int flipOffset;
    sf::Texture boardTexture_;
    sf::Sprite boardSprite_;
    sf::Window& bigWindow; //only to know its position
    resourceManager& resources;
    piecesBimap pieces;
    bool flipped() const;
    std::vector<std::vector<sf::FloatRect> > rectGrid;
    piecesBimap::iterator currentPiece;
    bool pieceHeld();
    void releasePiece();
    int idCount; //pieces ids

    void destroy(const int row, const int col); //will destroy the sprite in given location

    sf::Vector2f cellToPosition(const int row, const int col) const;

    sf::Vector2f getMousePosition() const; //mouse position in the canvas' coords

    void sendBack(); //sends the current piece back

    void flipBoard();

    //move was castle with destination the given square
    //so move rook to appropriate square
    void handleCastle(const int row, const int col);

    //move was en passant moving onto given square
    //so destroy the real pawn
    void handleEnPassant(const int row, const int col);

    //slots
    void slotMouseMove();
    void slotMouseRelease();
    void slotLeftClick();
    void slotEnterCanvas();

    void initPieces();
    void resetRects();

public:
    boardCanvas(sf::Window& theWindow);
    void display();
    void moveMake();

    sfg::Widget::Ptr getBoardWidget() const;
};

#endif // BOARDCANVAS_H
