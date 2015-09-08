#pragma once

#include <SFML/Graphics/Texture.hpp>

#include <SFGUI/Canvas.hpp>

#include <Thor/Shapes/Arrow.hpp>
#include <Thor/Particles/ParticleSystem.hpp>

#include <boost/signals2.hpp>
#include <boost/bimap.hpp>

#include "../views/components/PieceSprite.hpp"
#include "../BlackWolf.hpp"
#include "../Entity.hpp"

/*
 * A class aggregating a canvas widget and a bimap where the pieces are stored and mapped 1-1 to squares on the board.
 * The class is designed around the dimensions of Peter Wong's chess board, which is 440x440 - 50x50 squares,
 * with border width of 20.
 */


class Canvas : private Entity
{
public:
    Canvas(sf::Window& theWindow);
    void display();
    void setupBoard(const std::vector<std::vector<Unit> > &position, Color turnColor);

    sfg::Widget::Ptr getBoardWidget() const;

    void flipBoard();

    void setPremove(const Move& move);
    void clearArrows();

    void releasePiece();

    Color getColorOn(const Square& square) const;

    boost::signals2::signal<bool (const Move&)> requestMove;

private:

    //bimap
    struct squareId  {};
    struct pieceId {};
    using SquaresToPieces = boost::bimap<boost::bimaps::tagged<Square, squareId>, boost::bimaps::tagged<PieceSprite, pieceId>>;
    SquaresToPieces m_pieces;
    SquaresToPieces::const_iterator m_currentPiece;

    int m_flipOffset; //set to 0 for white on bottom, 50 for black
    static const sf::Vector2f m_offToCenter; //offset to center of square

    sfg::Canvas::Ptr m_canvas;
    sf::Sprite m_boardSprite;
    sf::Window& m_applicationWindow;

    thor::ParticleSystem m_particleSystem; //for capture animations

    std::vector<sf::Vertex> m_pieceVertices;
    sf::Texture m_piecesTexture, m_boardTexture;

    sf::Clock m_frameClock;

    sf::Vector2f m_pieceOffset; //offset from mouse tip to piece

    Color m_previousTurnColor;

    int m_idCount; //unique id for each piece

    std::vector<thor::Arrow> m_arrows;

    //slots
    void slotMouseMove();
    void slotMouseRelease();
    void slotLeftClick();
    void slotEnterCanvas();


    bool flipped() const;
    bool pieceHeld() const;

    void animateCaptureOn(const Square &square);

    Square positionToSquare(const sf::Vector2f& Position) const;
    sf::Vector2f squareToPosition(const Square& square) const;

    sf::Vector2f getMousePosition() const;

    void sendBack();

    void resetFor(Color whoFaceUp);

};
