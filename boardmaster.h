#ifndef BOARDMASTER_H
#define BOARDMASTER_H
#include <SFML/Graphics.hpp>
#include <SFGUI/Canvas.hpp>
#include <SFGUI/Label.hpp>
#include <SFGUI/Table.hpp>
#include <Thor/Time/CallbackTimer.hpp>
#include "piecesbimap.h"
#include "piecesprite.h"
#include "position.h"
#include "completemove.h"
#include "chessengine.h"





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

    cellsNpieces pieces;

    int humanColor;
    bool humanBoth; //if both players are human
    bool gameEnded;
    void setGameEnded(const int result);
    chessEngine chessAi;

    //std::map<squareId, pieceSprite> pieces;
    std::vector<std::vector<sf::FloatRect> > rectGrid;

    cellsNpieces::right_iterator currentPiece;
    bool pieceHeld();
    void releasePiece();
    //pieceSprite *currentPiece;
    sf::Vector2f clickedPoint;

    thor::CallbackTimer whiteClock;
    thor::CallbackTimer blackClock;

    void flagDown(const int side);

    int plyCounter;

    void handleCastle(const int row, const int col); //move was castle with destination the given square
    void handleEnPassant(const int row, const int col);
    void handlePromotion(const int row, const int col);

    void moveMake (const completeMove &move);

    void destroy(const int row, const int col); //will destroy the sprite in given location


public:
    boardMaster(sf::Window &theWindow);
    ~boardMaster();

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

    //modify functions
    pieceSprite changePosition(pieceSprite piece, const sf::Vector2f position) const; //returns a new sprite with changed position

};

#endif // BOARDMASTER_H
