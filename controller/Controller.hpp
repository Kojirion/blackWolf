#pragma once
#include "../views/Canvas.hpp"
#include "../model/GameData.hpp"
#include "../views/Status.hpp"
#include "../views/Clock.hpp"
#include "../views/MoveList.hpp"
#include "../controller/components/ColorChoices.hpp"
#include "../controller/components/Settings.hpp"
#include "../Client.hpp"
#include "../NetWidgets.hpp"
#include "../messages/Entity.hpp"
#include "../Actions.hpp"
#include "../Common/Move.hpp"


class Controller : private Messages::Registrant
{
public:
    Controller(sf::Window& window, sfg::Desktop& desktop,
               CallbackSystem& callbackSystem);

    sfg::Desktop& desktop;

    sfg::Window::Ptr boardWindow;
    sfg::Button::Ptr settingsButton;

    void update(sf::Time dt);
    void setEvent(const sf::Event& event);

    void updateClocks();

    //boost slots
    bool requestMove(const Move& move);

private:
    Canvas canvas;
    GameData game;
    Status status;
    Clock::Ptr m_whiteClock, m_blackClock;
    MoveList moveList;
    Settings settingsWindow;
    Move premove;
    bool premoveOn;
    sf::Event m_currentEvent;
    NetWidgets netWindow;
    sfg::Label::Ptr player1;
    sfg::Label::Ptr player2;    
    Client client;
};
