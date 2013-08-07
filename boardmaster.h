#ifndef BOARDMASTER_H
#define BOARDMASTER_H
#include <SFML/Graphics.hpp>
#include <SFGUI/Canvas.hpp>
#include <SFGUI/Label.hpp>
#include <SFGUI/Table.hpp>
#include <Thor/Time/Timer.hpp>
#include "piecesprite.h"
#include "position.h"
#include "completemove.h"

class boardMaster
{
private:
    sf::Window &bigWindow; //only to know its position

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

    int plyCounter;


public:
    boardMaster(sf::Window &theWindow);

    sfg::Canvas::Ptr window_;
    sfg::Label::Ptr turnLabel_;
    sfg::Canvas::Ptr whiteClockCanvas_;
    sfg::Canvas::Ptr blackClockCanvas_;
    sf::Text whiteClockText;
    sf::Text blackClockText;
    sfg::Table::Ptr moveList;

    void display();

    sf::Vector2f cellToPosition(const int row, const int col) const;
    const sf::Texture &idToTexture(const int pieceId) const;

    sf::Vector2f getMousePosition(); //mouse position in the canvas' coords

    int getTurnColor() const;

    void switchTurn();

    void sendBack(); //sends the current piece back

    std::string toString(sf::Time value) const;
    std::string colToString(const int col) const;
    std::string moveToString(const int row1, const int col1, const int row2, const int col2) const;
    std::string cellToString(const int row, const int col) const;

    void updateClocks();

    //slots
    void processLeftClick();
    void processMouseMove();
    void processMouseRelease();
    void processEnterCanvas();

};

#endif // BOARDMASTER_H
