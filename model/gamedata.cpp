#include "gamedata.h"
#include <boost/assert.hpp>

gameData::gameData():
    userColor(bw::None),
    result(bw::White | bw::Black), //init a game 'finished' as draw
    plyCounter(0)
{
    //whiteClock.connect(std::bind(&gameData::setResult, this, bw::Black));
    //blackClock.connect(std::bind(&gameData::setResult, this, bw::White));   
}

void gameData::setTime(int whiteTime, int blackTime)
{
    whiteClock.reset(sf::seconds(whiteTime));
    blackClock.reset(sf::seconds(blackTime));
}

bool gameData::ended() const
{
    return (result != bw::None);
}

bool gameData::userTurn() const
{
    return check(turnColor() & userColor);
}

bool gameData::userBoth() const
{
    return (userColor == (bw::White | bw::Black));
}

bw gameData::turnColor() const
{
    if (currentPosition.turnColor==1) return bw::White;
    return bw::Black;
}

bw gameData::getUserColor() const
{
    return userColor;
}

int gameData::getPlyCount() const
{
    return plyCounter;
}

const position gameData::getPosition() const
{
    return currentPosition;
}

void gameData::setPosition(position toSet)
{
    currentPosition = toSet;
}

void gameData::setResult(const bw winner)
{
    result = winner;
}

void gameData::newGame(const bw whoUser, int time)
{
    plyCounter = 0;

    currentPosition = position();

    userColor = whoUser;

    result = bw::None;

    setTime(time,time);
    whiteClock.start();

    //update
}

void gameData::update()
{
    whiteClock.update();
    blackClock.update();
}

void gameData::switchTurn()
{
    startClock();
    plyCounter++;
}

void gameData::setPromotion(const int row, const int col, const int piece)
{
    currentPosition.setPromotion(row, col, piece);
}

sf::Time gameData::getWhiteTime() const
{
    return whiteClock.getRemainingTime();
}

sf::Time gameData::getBlackTime() const
{
    return blackClock.getRemainingTime();
}

thor::CallbackTimer& gameData::getWhiteTimer()
{
    return whiteClock;
}

thor::CallbackTimer& gameData::getBlackTimer()
{
    return blackClock;
}

void gameData::startClock()
{
    if (turnColor() == bw::White){
        blackClock.stop();
        whiteClock.start();
    }else{
        BOOST_ASSERT_MSG(turnColor()==bw::Black, "Turn color invalid");
        whiteClock.stop();
        blackClock.start();
    }
}
