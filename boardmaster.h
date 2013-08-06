#ifndef BOARDMASTER_H
#define BOARDMASTER_H
#include <SFML/Graphics.hpp>
#include <SFGUI/Canvas.hpp>
#include "piecesprite.h"
#include "position.h"

class boardMaster
{
private:
    int turnColor;

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


public:
    boardMaster(sf::Window &theWindow);

    sfg::Canvas::Ptr window_;
    sf::Window &bigWindow; //only to know its position

    void display();

    sf::Vector2f cellToPosition(const int row, const int col);

    sf::Vector2f getMousePosition(); //mouse position in the canvas' coords

    int getTurnColor() const;

    //slots
    void processLeftClick();
    void processMouseMove();
    void processMouseRelease();
};

#endif // BOARDMASTER_H
