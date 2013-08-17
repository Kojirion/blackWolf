#ifndef BOARDMASTER_H
#define BOARDMASTER_H
#include <SFML/Graphics.hpp>
#include <SFGUI/Canvas.hpp>
#include <SFGUI/Label.hpp>
#include <SFGUI/Table.hpp>
#include <SFGUI/Window.hpp>
#include <SFGUI/Desktop.hpp>
#include <SFGUI/Button.hpp>
#include <Thor/Time/CallbackTimer.hpp>
#include "piecesbimap.h"
#include "piecesprite.h"
#include "position.h"
#include "completemove.h"
#include "chessengine.h"
#include "resourcemanager.h"





class boardMaster
{
private:
    sf::Window &bigWindow; //only to know its position

    sf::Font font;
    resourceManager resources;

    int flipOffset;

    sf::Texture boardTexture_;
    sf::Sprite boardSprite_;    
    position currentPosition;



    piecesBimap pieces;

    int humanColor;
    bool humanBoth; //if both players are human
    bool gameEnded;
    void setGameEnded(const int result);
    chessEngine chessAi;

    bool flipped() const;

    //std::map<squareId, pieceSprite> pieces;
    std::vector<std::vector<sf::FloatRect> > rectGrid;

    piecesBimap::iterator currentPiece;
    bool pieceHeld();
    void releasePiece();
    sf::Vector2f clickedPoint;

    thor::CallbackTimer whiteClock;
    thor::CallbackTimer blackClock;

    void flagDown(const int side);

    int plyCounter;
    int idCount; //pieces ids

    void handleCastle(const int row, const int col); //move was castle with destination the given square
    void handleEnPassant(const int row, const int col);
    void handlePromotion(const int row, const int col);

    void moveMake (const completeMove &move);

    void destroy(const int row, const int col); //will destroy the sprite in given location

    void newGame(const int whoHuman);

    void initPieces();
    void resetRects();

    void aiTurn();

    int toPromoteRow;
    int toPromoteCol;
    int promotionChoice; //which piece was chosen by either ai or player


public:
    boardMaster(sf::Window& theWindow, sfg::Desktop& theDesktop);
    ~boardMaster();

    sfg::Desktop& desktop;

    sfg::Canvas::Ptr window_;
    sfg::Label::Ptr turnLabel_;
    sfg::Canvas::Ptr whiteClockCanvas_;
    sfg::Canvas::Ptr blackClockCanvas_;
    sf::Text whiteClockText;
    sf::Text blackClockText;
    sfg::Table::Ptr moveList;
    sfg::Window::Ptr choiceWindow;
    sfg::Window::Ptr boardWindow;
    sfg::Window::Ptr sideChoiceWindow;
    sfg::Button::Ptr settingsButton;

    void display();

    sf::Vector2f cellToPosition(const int row, const int col) const;    

    sf::Vector2f getMousePosition(); //mouse position in the canvas' coords

    int getTurnColor() const;

    void switchTurn();

    void sendBack(); //sends the current piece back

    std::string timeToString(const sf::Time value) const;
    std::string colToString(const int col) const;
    std::string moveToString(const int row1, const int col1, const int row2, const int col2) const;
    std::string cellToString(const int row, const int col) const;

    void updateClocks();

    //slots
    void processLeftClick();
    void processMouseMove();
    void processMouseRelease();
    void processEnterCanvas();
    void flipBoard();
    void resign();
    void offerDraw();
    void requestNewGame();
    void promotionChoiceMade(const int whichPiece);
    void promoteQueen();
    void promoteBishop();
    void promoteKnight();
    void promoteRook();

    void whiteNewGame();
    void blackNewGame();
    void bothNewGame();


};

#endif // BOARDMASTER_H
