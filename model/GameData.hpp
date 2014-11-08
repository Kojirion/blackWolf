#pragma once
#include <Thor/Time/CallbackTimer.hpp>
#include "../BlackWolf.hpp"
#include "../Entity.hpp"

class GameData : private Entity
{
private:
    thor::CallbackTimer whiteClock;
    thor::CallbackTimer blackClock;
    Color m_userColor;
    Color m_turnColor;



public:
    GameData();

    bool userTurn() const;
    Color getUserColor() const;

    void update();

    void setTime(int whiteTime, int blackTime);

    sf::Time getWhiteTime() const;
    sf::Time getBlackTime() const;

    thor::CallbackTimer &getWhiteTimer();
    thor::CallbackTimer &getBlackTimer();

    void startClock(); //starts the appropriate clock
};
