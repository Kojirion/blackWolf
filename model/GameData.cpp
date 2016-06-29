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
