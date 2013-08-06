#ifndef BOARDMASTER_H
#define BOARDMASTER_H
#include <SFML/Graphics.hpp>
#include <SFGUI/Canvas.hpp>
#include <SFGUI/Label.hpp>
#include <Thor/Time/Timer.hpp>
#include "piecesprite.h"
#include "position.h"

class boardMaster
{
private:
    sf::Window &bigWindow; //only to know its position

    int turnColor;

    sf::Font font;

    sf::Vector2f flipOffset;

    sf::Texture boardTexture_;
    sf::Sprite boardSprite_;    
    position currentPosition;

    sf::Texture blackRookT;
    sf::Texture blackBishopT;
    sf::Texture blackKnightT;
    sf::Texture blackQueenT;
    sf::Texture blackKingT;
    sf::Texture blackPawnT;
    sf::Texture whiteRookT;
    sf::Texture whiteBishopT;
    sf::Texture whiteKnightT;
    sf::Texture whiteQueenT;
    sf::Texture whiteKingT;
    sf::Texture whitePawnT;    

    std::vector<pieceSprite> pieces;
    std::vector<std::vector<int> > cells;

    std::vector<std::vector<sf::FloatRect> > rectGrid;

    pieceSprite *currentPiece;
    sf::Vector2f clickedPoint;

    thor::Timer whiteClock;
    thor::Timer blackClock;


public:
    boardMaster(sf::Window &theWindow);

    sfg::Canvas::Ptr window_;
    sfg::Label::Ptr turnLabel_;
    sfg::Canvas::Ptr whiteClockCanvas_;
    sfg::Canvas::Ptr blackClockCanvas_;
    sf::Text whiteClockText;
    sf::Text blackClockText;

    void display();

    sf::Vector2f cellToPosition(const int row, const int col);

    sf::Vector2f getMousePosition(); //mouse position in the canvas' coords

    int getTurnColor() const;

    void switchTurn();

    std::string toString(sf::Time value) const;
    void updateClocks();

    //slots
    void processLeftClick();
    void processMouseMove();
    void processMouseRelease();

};

#endif // BOARDMASTER_H
