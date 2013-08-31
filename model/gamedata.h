#ifndef GAMEDATA_H
#define GAMEDATA_H
#include <Thor/Time/CallbackTimer.hpp>
#include "blackWolf.h"
#include "common/position.h"

class gameData
{
private:
    position currentPosition;
    bw userColor; //color(s) of user
    bw result; //who won

    thor::CallbackTimer whiteClock;
    thor::CallbackTimer blackClock;

    int plyCounter;

public:
    gameData();

    bool ended() const;
    bool userTurn() const;
    bool userBoth() const;

    bw turnColor() const; //whose turn
    bw getUserColor() const;

    int getPlyCount() const;

    const position getPosition() const;
    void setPosition(position toSet);

    void setResult(const bw winner);
    void newGame(const bw whoUser);
    void update();
    void switchTurn();

    void setPromotion(const int row, const int col, const int piece);

    sf::Time getWhiteTime() const;
    sf::Time getBlackTime() const;

    thor::CallbackTimer &getWhiteTimer();
    thor::CallbackTimer &getBlackTimer();
};

#endif // GAMEDATA_H
