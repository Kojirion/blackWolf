#ifndef GAMEDATA_H
#define GAMEDATA_H
#include <Thor/Time/CallbackTimer.hpp>
#include "../BlackWolf.hpp"
#include "../common/Position.hpp"
#include "../Entity.hpp"

class GameData : private Entity
{
private:
    std::vector<Position> m_positions;
    Color m_userColor; //color(s) of user
    Color m_result; //who won

    thor::CallbackTimer whiteClock;
    thor::CallbackTimer blackClock;

    unsigned int plyCounter;
    unsigned int fiftyMovePlyCounter;

    void setResult(Color winner);

public:
    GameData();

    bool ended() const;
    bool userTurn() const;
    bool userBoth() const;

    Color turnColor() const; //whose turn
    Color getUserColor() const;

    int getPlyCount() const;

    const Position& getPosition() const;
    void setPosition(const Position &toSet);


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
