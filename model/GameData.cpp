#include "GameData.hpp"
#include <boost/assert.hpp>
#include <boost/cast.hpp>
#include "../controller/BoardMaster.hpp"

GameData::GameData():
    m_userColor(Color::None),
    m_result(Color::Both), //init a game 'finished' as draw
    plyCounter(0)
{
    //whiteClock.connect(std::bind(&gameData::setResult, this, bw::Black));
    //blackClock.connect(std::bind(&gameData::setResult, this, bw::White));

    messageSystem.connect("moveMade", [this](const Message& message){
        const MoveMessage* received = boost::polymorphic_downcast<const MoveMessage*>(&message);
        setPosition(received->move.getNewBoard());
        if (!ended())
            switchTurn();
    });
}

void GameData::setTime(int whiteTime, int blackTime)
{
    whiteClock.reset(sf::seconds(whiteTime));
    blackClock.reset(sf::seconds(blackTime));
}

bool GameData::ended() const
{
    return (m_result != Color::None);
}

bool GameData::userTurn() const
{
    return (turnColor() & m_userColor);
}

bool GameData::userBoth() const
{
    return (m_userColor == Color::Both);
}

Color GameData::turnColor() const
{
    return m_position.getTurnColor();
}

Color GameData::getUserColor() const
{
    return m_userColor;
}

int GameData::getPlyCount() const
{
    return plyCounter;
}

const Position &GameData::getPosition() const
{
    return m_position;
}

void GameData::setPosition(Position toSet)
{
    m_position = toSet;
}

void GameData::setResult(Color winner)
{
    m_result = winner;
}

void GameData::newGame(Color whoUser, int time)
{
    plyCounter = 0;

    m_position = Position();

    m_userColor = whoUser;

    m_result = Color::None;

    setTime(time,time);
    whiteClock.start();

    //update
}

void GameData::update()
{
    whiteClock.update();
    blackClock.update();
}

void GameData::switchTurn()
{
    startClock();
    plyCounter++;
}

void GameData::setPromotion(const Square& square, Unit piece)
{
    m_position.setPromotion(square, piece);
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
    if (turnColor() == Color::White){
        blackClock.stop();
        whiteClock.start();
    }else{
        BOOST_ASSERT_MSG(turnColor()==Color::Black, "Turn color invalid");
        whiteClock.stop();
        blackClock.start();
    }
}
