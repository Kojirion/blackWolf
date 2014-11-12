#include "GameData.hpp"
#include <boost/assert.hpp>
#include <boost/cast.hpp>
#include "../controller/Controller.hpp"

GameData::GameData()
{
    messages.connect("newGame", [this](const Message& message){
        auto received = boost::polymorphic_downcast<const NewGameMessage*>(&message);
        m_userColor = received->user;
    });

    messages.connect("gameState", [this](const Message& message){
        auto received = boost::polymorphic_downcast<const GameStateMessage*>(&message);
        m_turnColor = received->turnColor;
    });
}

bool GameData::userTurn() const
{
    return m_userColor & m_turnColor;
}

Color GameData::getUserColor() const
{
    return m_userColor;
}

void GameData::setTime(int whiteTime, int blackTime)
{
    whiteClock.reset(sf::seconds(whiteTime));
    blackClock.reset(sf::seconds(blackTime));
}

sf::Time GameData::getWhiteTime() const
{
    return whiteClock.getRemainingTime();
}

sf::Time GameData::getBlackTime() const
{
    return blackClock.getRemainingTime();
}

void GameData::startClock()
{
    if (m_turnColor == Color::White){
        blackClock.stop();
        whiteClock.start();
    }else{
        BOOST_ASSERT_MSG(m_turnColor==Color::Black, "Turn color invalid");
        whiteClock.stop();
        blackClock.start();
    }
}
