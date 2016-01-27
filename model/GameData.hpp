#pragma once
#include <Thor/Time/Timer.hpp>
#include "../BlackWolf.hpp"
#include "../messages/Entity.hpp"

class GameData : private Messages::Registrant
{
private:
    thor::Timer whiteClock, blackClock;
    Color m_userColor;
    Color m_turnColor;

public:
    GameData();

    bool userTurn() const;
    Color getUserColor() const;

    void setTime(int whiteTime, int blackTime);

    sf::Time getWhiteTime() const;
    sf::Time getBlackTime() const;

    void startClock(); //starts the appropriate clock
};
