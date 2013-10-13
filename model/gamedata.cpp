#include "gamedata.h"
#include <boost/assert.hpp>

gameData::gameData():
    m_userColor(Color::None),
    m_result(Color::Both), //init a game 'finished' as draw
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
    return (m_result != Color::None);
}

bool gameData::userTurn() const
{
    return (turnColor() & m_userColor);
}

bool gameData::userBoth() const
{
    return (m_userColor == Color::Both);
}

Color gameData::turnColor() const
{
    return m_position.getTurnColor();
}

Color gameData::getUserColor() const
{
    return m_userColor;
}

int gameData::getPlyCount() const
{
    return plyCounter;
}

const position &gameData::getPosition() const
{
    return m_position;
}

void gameData::setPosition(position toSet)
{
    m_position = toSet;
}

void gameData::setResult(Color winner)
{
    m_result = winner;
}

void gameData::newGame(Color whoUser, int time)
{
    plyCounter = 0;

    m_position = position();

    m_userColor = whoUser;

    m_result = Color::None;

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

void gameData::setPromotion(const Square& square, Unit piece)
{
    m_position.setPromotion(square, piece);
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
    if (turnColor() == Color::White){
        blackClock.stop();
        whiteClock.start();
    }else{
        BOOST_ASSERT_MSG(turnColor()==Color::Black, "Turn color invalid");
        whiteClock.stop();
        blackClock.start();
    }
}
