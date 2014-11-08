#include "GameData.hpp"
#include <boost/assert.hpp>
#include <boost/cast.hpp>
#include "../controller/Controller.hpp"

GameData::GameData()
{
    //whiteClock.connect(std::bind(&gameData::setResult, this, bw::Black));
    //blackClock.connect(std::bind(&gameData::setResult, this, bw::White));

    messages.connect("moveMade", [this](const Message& message){
        const MoveMessage* received = boost::polymorphic_downcast<const MoveMessage*>(&message);
//        if (received->move.fiftyMoveValid)
//            ++fiftyMovePlyCounter;
//        else fiftyMovePlyCounter = 0;
        //messages.triggerEvent(CountMessage(std::count(m_positions.begin(), m_positions.end(), m_positions.back()), fiftyMovePlyCounter/2));
//        if (!ended())
//            switchTurn();
    });

    messages.connect("newGame", [this](const Message& message){
        auto received = boost::polymorphic_downcast<const NewGameMessage*>(&message);
        m_userColor = received->user;
    });

    messages.connect("gameState", [this](const Message& message){
        auto received = boost::polymorphic_downcast<const GameStateMessage*>(&message);
        m_turnColor = received->turnColor;
    });

    messages.connect("endGame", [this](const Message& message){
        const EndGameMessage* received = boost::polymorphic_downcast<const EndGameMessage*>(&message);
        //setResult(received->result);
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

void GameData::update()
{
    whiteClock.update();
    blackClock.update();
}

sf::Time GameData::getWhiteTime() const
{
    return whiteClock.getRemainingTime();
}

sf::Time GameData::getBlackTime() const
{
    return blackClock.getRemainingTime();
}

thor::CallbackTimer& GameData::getWhiteTimer()
{
    return whiteClock;
}

thor::CallbackTimer& GameData::getBlackTimer()
{
    return blackClock;
}

void GameData::startClock()
{
    //if (turnColor() == Color::White){
        blackClock.stop();
        whiteClock.start();
//    }else{
//        BOOST_ASSERT_MSG(turnColor()==Color::Black, "Turn color invalid");
//        whiteClock.stop();
//        blackClock.start();
//    }
}
