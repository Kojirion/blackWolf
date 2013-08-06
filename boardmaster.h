#ifndef BOARDMASTER_H
#define BOARDMASTER_H
#include <SFML/Graphics.hpp>
#include <SFGUI/Canvas.hpp>
#include "piecesprite.h"
#include "position.h"

class boardMaster
{
private:
    sf::Vector2f offset;
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


public:
    boardMaster();

    sfg::Canvas::Ptr window_;

    void display();

    sf::Vector2f cellToPosition(const int row, const int col);
};

#endif // BOARDMASTER_H
