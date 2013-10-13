//the board "view"
//a class built around sfg::Canvas

#ifndef BOARDCANVAS_H
#define BOARDCANVAS_H
#include <SFGUI/Canvas.hpp>
#include <boost/signals2.hpp>
#include <boost/bimap.hpp>
#include "../views/components/piecesprite.h"
#include "../resourcemanager.h"
#include "../common/completemove.h"
#include "../common/position.h"
#include "../blackWolf.h"
#include "../views/components/firework.h"
#include <Thor/Shapes/Arrow.hpp>

class boardCanvas
{
private:

    struct squareId  {};
    struct pieceId {};

    typedef boost::bimap<boost::bimaps::tagged<Square, squareId>, boost::bimaps::tagged<pieceSprite, pieceId> > SquaresToPieces;

    int flipOffset;

    static const sf::Vector2f offToCenter;

    sfg::Canvas::Ptr window;

    sf::Sprite boardSprite_;
    sf::Window& bigWindow;

    resourceManager& resources;

    SquaresToPieces pieces;

    std::vector<sf::Vertex> vertexArray;
    sf::Texture piecesTexture;

    bool flipped() const;

    sf::Texture particle;
    std::unique_ptr<thor::ParticleSystem> system;    
    sf::Clock frameClock; //redundant?

    sf::Vector2i toGridPos(const sf::Vector2f& position) const;

    SquaresToPieces::const_iterator currentPiece;

    bool pieceHeld();

    int idCount;

    void destroy(const Square &square);

    sf::Vector2f cellToPosition(const Square& square) const;

    sf::Vector2f getMousePosition() const;

    void sendBack();

    //move was castle with destination the given square
    //so move rook to appropriate square
    void handleCastle(const Square& square);

    //move was en passant moving onto given square
    //so destroy the real pawn
    void handleEnPassant(const Square& square);

    //signals
    boost::signals2::signal<bool (const Move&)> requestMove;

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
    void moveMake(const completeMove &move);

    void bimapMove(const Move &move);

    sfg::Widget::Ptr getBoardWidget() const;
    void setPosition(const position &givenPosition);
    void resetFor(Color whoFaceUp);

    boost::signals2::signal<bool (const Move &)> &getSignal();

    void setPromotion(const Square& square, Piece piece);

    void flipBoard();

    void reload(const position &givenPosition);

    void setResult(Color result);

    void setArrow(const Move& move);
    void clearArrows();

    void releasePiece();
};

#endif // BOARDCANVAS_H
