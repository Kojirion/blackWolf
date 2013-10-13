#ifndef GAMEDATA_H
#define GAMEDATA_H
#include <Thor/Time/CallbackTimer.hpp>
#include "../blackWolf.h"
#include "../common/position.h"

class gameData
{
private:
    position m_position;
    Color m_userColor; //color(s) of user
    Color m_result; //who won

    thor::CallbackTimer whiteClock;
    thor::CallbackTimer blackClock;

    int plyCounter;

public:
    gameData();

    bool ended() const;
    bool userTurn() const;
    bool userBoth() const;

    Color turnColor() const; //whose turn
    Color getUserColor() const;

    int getPlyCount() const;

    const position& getPosition() const;
    void setPosition(position toSet);

    void setResult(Color winner);
    void newGame(Color whoUser, int time);
    void update();
    void switchTurn();

    void setPromotion(const Square &square, Unit piece);
    void setTime(int whiteTime, int blackTime);

    sf::Time getWhiteTime() const;
    sf::Time getBlackTime() const;

    thor::CallbackTimer &getWhiteTimer();
    thor::CallbackTimer &getBlackTimer();

    void startClock(); //starts the appropriate clock
};

#endif // GAMEDATA_H
