#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "../common/Position.hpp"
#include "../common/CompleteMove.hpp"
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
#include "../views/Counters.hpp"
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

    Counters counters;

    ColorChoices sideChoice;

    Settings settingsWindow;

    Move premove;
    bool premoveOn;

    NetWidgets netWindow;

    sfg::Label::Ptr player1;
    sfg::Label::Ptr player2;    
    Client client;

    void handlePromotion(const Move &move);

    Move toPromote;
    Piece promotionChoice;


    void settingsClicked();
    void settingsDone(const std::string &whitePrefix, const std::string &blackPrefix, const std::string &boardSuffix);


    void flagDown(Color loser);

    void moveMake(const Move& move, int whiteTime, int blackTime, Piece promotionChoice = Piece::None);
    void newGame(Color player, int time, const std::string& player_1, const std::string& player_2);

    void enableWindow(const bool enable = true);

};

#endif // CONTROLLER_H
