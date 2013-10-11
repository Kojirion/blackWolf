//'Controller' class
//owns a gameData 'model' and several 'view' classes

#ifndef BOARDMASTER_H
#define BOARDMASTER_H
#include "../common/position.h"
#include "../common/completemove.h"
#include "../chessengine.h"
#include "../resourcemanager.h"
#include "../views/boardcanvas.h"
#include "../model/gamedata.h"
#include "../views/statusview.h"
#include "../views/clocksview.h"
#include "../views/moveview.h"
#include "../controller/components/sidechoicewindow.h"
#include "../controller/components/settingsstate.h"
#include "../client.h"
#include "../netwidgets.h"





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
    void promotionChoiceMade(Piece whichPiece);
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

    typedef std::tuple<bool,int,int,int,int> premoveTuple;

    premoveTuple premove;

    netWidgets netWindow;

    sfg::Label::Ptr player1;
    sfg::Label::Ptr player2;

    chessEngine chessAi;
    client fics;

    //move was promotion
    //so pop the window for selection or have ai choose
    void handlePromotion(int row1, int col1, int row2, int col2);

    int toPromoteRow1;
    int toPromoteCol1;
    int toPromoteRow2;
    int toPromoteCol2;
    Piece promotionChoice; //which piece was chosen by either ai or player


    void settingsClicked();
    void settingsDone(std::string whitePrefix, std::string blackPrefix, std::string boardSuffix);


    void flagDown(Color loser);

    void moveMake (const completeMove &move);
    void networkMoveMake (int row1, int col1, int row2, int col2, int whiteTime, int blackTime, Piece promotionChoice);



    void newGame(Color whoUser, int time, std::string p1, std::string p2);

    void aiTurn();

    void setGameEnded(Color result);

    void enableWindow(const bool enable = true);

};

#endif // BOARDMASTER_H
