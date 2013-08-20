#ifndef BOARDMASTER_H
#define BOARDMASTER_H
#include <SFML/Graphics.hpp>
#include <SFGUI/Canvas.hpp>
#include <SFGUI/Label.hpp>
#include <SFGUI/Table.hpp>
#include <SFGUI/Window.hpp>
#include <SFGUI/Desktop.hpp>
#include <SFGUI/Button.hpp>
#include "position.h"
#include "completemove.h"
#include "chessengine.h"
#include "resourcemanager.h"
#include "boardcanvas.h"
#include "gamedata.h"





class boardMaster
{
private:

    resourceManager resources;

    boardCanvas board;

    position currentPosition;

    gameData game;






    chessEngine chessAi;


    //move was promotion
    //so pop the window for selection or have ai choose
    void handlePromotion(const int row, const int col);

    int toPromoteRow;
    int toPromoteCol;
    int promotionChoice; //which piece was chosen by either ai or player






    void flagDown(const int side);

    int plyCounter;

    void moveMake (const completeMove &move);



    void newGame(const int whoHuman);

    void initPieces();
    void resetRects();

    void aiTurn();


public:
    boardMaster(sf::Window& theWindow, sfg::Desktop& theDesktop);
    ~boardMaster();

    sfg::Desktop& desktop;
    sfg::Window::Ptr promotionWindow;

    sfg::Label::Ptr turnLabel_;

    sfg::Window::Ptr boardWindow;
    sfg::Window::Ptr sideChoiceWindow;
    sfg::Button::Ptr settingsButton;

    void display();

    int getTurnColor() const;

    void switchTurn();



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
