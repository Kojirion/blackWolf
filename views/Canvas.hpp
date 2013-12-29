//the board "view"
//a class built around sfg::Canvas

#ifndef BOARDCANVAS_H
#define BOARDCANVAS_H
#include <SFGUI/Canvas.hpp>
#include <boost/signals2.hpp>
#include <boost/bimap.hpp>
#include "../views/components/PieceSprite.hpp"
#include "../Resources.hpp"
#include "../common/CompleteMove.hpp"
#include "../common/Position.hpp"
#include "../BlackWolf.hpp"
#include "../views/components/Firework.hpp"
#include <Thor/Shapes/Arrow.hpp>
#include "../Entity.hpp"

class Canvas : private Entity
{
private:

    struct squareId  {};
    struct pieceId {};

    typedef boost::bimap<boost::bimaps::tagged<Square, squareId>, boost::bimaps::tagged<PieceSprite, pieceId> > SquaresToPieces;

    int flipOffset;

    static const sf::Vector2f offToCenter;

    sfg::Canvas::Ptr window;

    sf::Sprite boardSprite_;
    sf::Window& bigWindow;

    Resources& resources;

    SquaresToPieces pieces;

    std::vector<sf::Vertex> vertexArray;
    sf::Texture piecesTexture;

    bool flipped() const;

    sf::Texture particle;
    std::unique_ptr<thor::ParticleSystem> system;    
    sf::Clock frameClock; //redundant?

    Square toGridPos(const sf::Vector2f& Position) const;

    SquaresToPieces::const_iterator currentPiece;

    bool pieceHeld();
    sf::Vector2f pieceOffset;

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

    std::vector<thor::Arrow> arrows;

    void setPosition(const Position &givenPosition);
    void resetFor(Color whoFaceUp);

    void setResult(Color result);

public:
    Canvas(sf::Window& theWindow, Resources &theResources);
    void display();
    void moveMake(const CompleteMove &move);

    void bimapMove(const Move &move);

    sfg::Widget::Ptr getBoardWidget() const;

    boost::signals2::signal<bool (const Move &)> &getSignal();

    void setPromotion(const Square& square, Piece piece);

    void flipBoard();

    void reload(const Position &givenPosition);    

    void setArrow(const Move& move);
    void clearArrows();

    void releasePiece();
};

#endif // BOARDCANVAS_H
