#ifndef GAMEDATA_H
#define GAMEDATA_H
#include <Thor/Time/CallbackTimer.hpp>
#include "blackWolf.h"
#include "position.h"

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

    bw turnColor() const; //whose turn
    bw getUserColor() const;

    const position getPosition() const;
    void setPosition(position toSet);

    void setResult(const bw winner);
    void newGame(const bw whoUser);
    void update();
    void switchTurn();

    sf::Time getWhiteTime() const;
    sf::Time getBlackTime() const;
};

#endif // GAMEDATA_H
