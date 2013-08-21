//'Controller' class
//owns a gameData 'model' and several 'view' classes

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
#include "views/boardcanvas.h"
#include "gamedata.h"
#include "views/statusview.h"
#include "views/clocksview.h"
#include "views/moveview.h"
#include "components/sidechoicewindow.h"
#include "settingsstate.h"





class boardMaster
{
public:
    boardMaster(sf::Window& theWindow, sfg::Desktop& theDesktop);
    ~boardMaster();

    sfg::Desktop& desktop;
    sfg::Window::Ptr promotionWindow;

    sfg::Window::Ptr boardWindow;
    sfg::Button::Ptr settingsButton;

    void display();

    void switchTurn();



    void updateClocks();

    //slots
    void processLeftClick();
    void processMouseMove();
    void processMouseRelease();
    void processEnterCanvas();
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

    //boost slots
    bool requestMove(int row1, int col1, int row2, int col2);

private:

    resourceManager resources;

    boardCanvas board;

    gameData game;

    statusView status;

    clocksView clocks;

    moveView moveList;

    sideChoiceWindow sideChoice;

    settingsState settingsWindow;






    chessEngine chessAi;


    //move was promotion
    //so pop the window for selection or have ai choose
    void handlePromotion(const int row, const int col);

    int toPromoteRow;
    int toPromoteCol;
    int promotionChoice; //which piece was chosen by either ai or player


    void settingsClicked();
    void settingsDone(std::string whitePrefix, std::string blackPrefix, std::string boardSuffix);


    void flagDown(const bw loser);

    void moveMake (const completeMove &move);



    void newGame(const bw whoUser);

    void aiTurn();

    void setGameEnded(bw result);

    void enableWindow(const bool enable = true);

};

#endif // BOARDMASTER_H
