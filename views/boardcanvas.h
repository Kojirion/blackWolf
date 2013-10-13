//the board "view"
//a class built around sfg::Canvas

#ifndef BOARDCANVAS_H
#define BOARDCANVAS_H
#include <SFGUI/Canvas.hpp>
#include <boost/signals2.hpp>
#include "../views/components/piecesbimap.h"
#include "../resourcemanager.h"
#include "../common/completemove.h"
#include "../common/position.h"
#include "../blackWolf.h"
#include "../views/components/firework.h"
#include <Thor/Shapes/Arrow.hpp>

class boardCanvas
{
private:
    //offset used in positioning the pieces to achieve flip
    int flipOffset;

    static const sf::Vector2f offToCenter;

    //Canvas onto which board is drawn
    sfg::Canvas::Ptr window;

    sf::Sprite boardSprite_;
    sf::Window& bigWindow; //only to know its position

    resourceManager& resources;

    //bimap between board squares and piece sprites
    piecesBimap pieces;

    std::vector<sf::Vertex> vertexArray;
    sf::Texture piecesTexture;

    bool flipped() const;

    //firework system
    sf::Texture particle;
    std::unique_ptr<thor::ParticleSystem> system;

    //a clock to update firework system - but possible to use the one from guimanager?
    sf::Clock frameClock;    

    sf::Vector2i toGridPos(const sf::Vector2f& position) const;

    //piece held by the mouse
    piecesBimap::iterator currentPiece;

    bool pieceHeld();    

    int idCount; //pieces ids for bimap's use

    void destroy(const int row, const int col); //will destroy the sprite in given location

    sf::Vector2f cellToPosition(const int row, const int col) const;

    sf::Vector2f getMousePosition() const; //mouse position in the canvas' coords

    void sendBack(); //sends the current piece back

    //move was castle with destination the given square
    //so move rook to appropriate square
    void handleCastle(const int row, const int col);

    //move was en passant moving onto given square
    //so destroy the real pawn
    void handleEnPassant(const int row, const int col);

    //signals
    boost::signals2::signal<bool (int, int, int, int)> requestMove;

    //slots
    void slotMouseMove();
    void slotMouseRelease();
    void slotLeftClick();
    void slotEnterCanvas();

    void resetRects();

    std::vector<thor::Arrow> arrows;

public:
    boardCanvas(sf::Window& theWindow, resourceManager &theResources);
    void display();
    void moveMake(const completeMove move);

    sfg::Widget::Ptr getBoardWidget() const;
    void setPosition(const position &givenPosition);
    void resetFor(Color whoFaceUp); //reset and flip if needed

    boost::signals2::signal<bool (int, int, int, int)> &getSignal();

    void setPromotion(int row, int col, Piece piece);

    void flipBoard();

    void reload(const position &givenPosition);

    void setResult(Color result);

    void setArrow(int row1, int col1, int row2, int col2);
    void clearArrows();

    void releasePiece();
};

#endif // BOARDCANVAS_H
