#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "../common/Position.hpp"
#include "../common/CompleteMove.hpp"
#include "../Engine.hpp"
#include "../Resources.hpp"
#include "../views/Canvas.hpp"
#include "../model/GameData.hpp"
#include "../views/Status.hpp"
#include "../views/Clocks.hpp"
#include "../views/MoveList.hpp"
#include "../controller/components/ColorChoices.hpp"
#include "../controller/components/Settings.hpp"
#include "../Client.hpp"
#include "../NetWidgets.hpp"
#include "../Entity.hpp"


class Controller : private Entity
{
public:
    Controller(sf::Window& theWindow, sfg::Desktop& theDesktop);

    sfg::Desktop& desktop;
    sfg::Window::Ptr promotionWindow;

    sfg::Window::Ptr boardWindow;
    sfg::Button::Ptr settingsButton;

    void update();

    void updateClocks();

    //slots
    void processLeftClick();
    void processMouseMove();
    void processMouseRelease();
    void processEnterCanvas();
    void resign();
    void offerDraw();
    void requestNewGame();

    void slotNewGame();
    void slotPromote();

    //boost slots
    bool requestMove(const Move& move);

private:

    Resources resources;

    Canvas board;

    GameData game;

    Status status;

    Clocks clocks;

    MoveList moveList;

    ColorChoices sideChoice;

    Settings settingsWindow;

    Move premove;
    bool premoveOn;

    NetWidgets netWindow;

    sfg::Label::Ptr player1;
    sfg::Label::Ptr player2;

    Engine chessAi;
    Client fics;

    void handlePromotion(const Move &move);

    Move toPromote;
    Piece promotionChoice;


    void settingsClicked();
    void settingsDone(std::string whitePrefix, std::string blackPrefix, std::string boardSuffix);


    void flagDown(Color loser);

    void moveMake (const CompleteMove &move);

    void newGame(Color whoUser, int time, std::string p1, std::string p2);

    void aiTurn();

    void setGameEnded(Color result);

    void enableWindow(const bool enable = true);

};

#endif // CONTROLLER_H
