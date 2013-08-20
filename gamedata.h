#ifndef GAMEDATA_H
#define GAMEDATA_H
#include <Thor/Time/CallbackTimer.hpp>
#include "blackWolf.h"

class gameData
{
private:
    bw turnColor; //whose turn
    bw userColor; //color(s) of user
    bw result; //who won

    thor::CallbackTimer whiteClock;
    thor::CallbackTimer blackClock;

    int plyCounter;

public:
    gameData();

    bool ended() const;
    bool userTurn() const;

    void setResult(const bw winner);
    void newGame(const bw whoUser);
    void update();
    void switchTurn();
};

#endif // GAMEDATA_H
