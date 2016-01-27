#include "GameData.hpp"
#include <cassert>
#include <boost/cast.hpp>
#include "../controller/Controller.hpp"
#include "../messages/GameState.hpp"
#include "../messages/GameStart.hpp"


GameData::GameData()
{
    messages.connect(Messages::ID::GameStart, [this](const Messages::Message& message){
        auto received = boost::polymorphic_downcast<const Messages::GameStart*>(&message);
        m_userColor = received->user;
    });

    messages.connect(Messages::ID::GameState, [this](const Messages::Message& message){
        auto received = boost::polymorphic_downcast<const Messages::GameState*>(&message);
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
        assert(m_turnColor==Color::Black);
        whiteClock.stop();
        blackClock.start();
    }
}
