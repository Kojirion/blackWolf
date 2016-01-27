#pragma once
#include "../views/Canvas.hpp"
#include "../model/GameData.hpp"
#include "../views/Status.hpp"
#include "../views/Clocks.hpp"
#include "../views/MoveList.hpp"
#include "../controller/components/ColorChoices.hpp"
#include "../controller/components/Settings.hpp"
#include "../Client.hpp"
#include "../NetWidgets.hpp"
#include "../messages/Entity.hpp"
#include "../Actions.hpp"


class Controller : private Messages::Entity
{
public:
    Controller(sf::Window& theWindow, sfg::Desktop& theDesktop,
               CallbackSystem& callbackSystem);

    sfg::Desktop& desktop;

    sfg::Window::Ptr boardWindow;
    sfg::Button::Ptr settingsButton;

    void update();
    void setEvent(const sf::Event& event);

    void updateClocks();

    //boost slots
    bool requestMove(const Move& move);

private:

    Canvas board;

    GameData game;

    Status status;

    Clocks clocks;

    MoveList moveList;

    Settings settingsWindow;

    Move premove;
    bool premoveOn;

    sf::Event m_currentEvent;

    NetWidgets netWindow;

    sfg::Label::Ptr player1;
    sfg::Label::Ptr player2;    
    Client client;

    void settingsClicked();
    void settingsDone(const PieceToTexPos& pieceToTexPos);
    void enableWindow(const bool enable = true);

};
