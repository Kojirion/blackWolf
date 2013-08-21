#include "gamedata.h"
#include <boost/assert.hpp>

gameData::gameData()
{
    whiteClock.connect(std::bind(&gameData::setResult, this, bw::Black));
    blackClock.connect(std::bind(&gameData::setResult, this, bw::White));
}

bool gameData::ended() const
{
    return (result != bw::None);
}

bool gameData::userTurn() const
{
    return check(turnColor() & userColor);
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

void gameData::newGame(const bw whoUser)
{
    plyCounter = 0;

    currentPosition = position();

    userColor = whoUser;

    result = bw::None;

    whiteClock.restart(sf::seconds(300.f));
    blackClock.restart(sf::seconds(300.f));
    blackClock.stop();

    //update
}

void gameData::update()
{
    whiteClock.update();
    blackClock.update();
}

void gameData::switchTurn()
{
    if (turnColor() == bw::White){
        blackClock.stop();
        whiteClock.start();
    }else{
        BOOST_ASSERT_MSG(turnColor()==bw::Black, "Turn color invalid");
        whiteClock.stop();
        blackClock.start();
    }
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
