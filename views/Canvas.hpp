#pragma once
#include <SFGUI/Canvas.hpp>
#include <boost/signals2.hpp>
#include <boost/bimap.hpp>
#include "../views/components/PieceSprite.hpp"
#include "../BlackWolf.hpp"
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

    SquaresToPieces pieces;

    std::vector<sf::Vertex> vertexArray;
    sf::Texture piecesTexture, boardTexture;

    bool flipped() const;

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

    //signals
    boost::signals2::signal<bool (const Move&)> requestMove;

    //slots
    void slotMouseMove();
    void slotMouseRelease();
    void slotLeftClick();
    void slotEnterCanvas();    

    std::vector<thor::Arrow> arrows;

    void resetFor(Color whoFaceUp);

public:
    Canvas(sf::Window& theWindow);
    void display();
    void setupBoard(const std::vector<std::vector<Unit> > &position);

    sfg::Widget::Ptr getBoardWidget() const;

    boost::signals2::signal<bool (const Move &)> &getSignal();

    void flipBoard();

    void setArrow(const Move& move);
    void clearArrows();

    void releasePiece();

    Color getColorOn(const Square& square) const;
};
