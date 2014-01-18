#include "GameData.hpp"
#include <boost/assert.hpp>
#include <boost/cast.hpp>
#include "../controller/Controller.hpp"

GameData::GameData():
    m_userColor(Color::None),
    m_result(Color::Both), //init a game 'finished' as draw
    plyCounter(0),
    fiftyMovePlyCounter(0)
{
    //whiteClock.connect(std::bind(&gameData::setResult, this, bw::Black));
    //blackClock.connect(std::bind(&gameData::setResult, this, bw::White));

    messages.connect("moveMade", [this](const Message& message){
        const MoveMessage* received = boost::polymorphic_downcast<const MoveMessage*>(&message);
        setPosition(received->move.getNewBoard());
        if (received->move.fiftyMoveValid)
            ++fiftyMovePlyCounter;
        else fiftyMovePlyCounter = 0;
        messages.triggerEvent(CountMessage(std::count(m_positions.begin(), m_positions.end(), m_positions.back()), fiftyMovePlyCounter/2));
        if (!ended())
            switchTurn();
    });

    messages.connect("newGame", [this](const Message& message){
        const NewGameMessage* received = boost::polymorphic_downcast<const NewGameMessage*>(&message);
        newGame(received->user, received->time);
    });

    messages.connect("endGame", [this](const Message& message){
        const EndGameMessage* received = boost::polymorphic_downcast<const EndGameMessage*>(&message);
        setResult(received->result);
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
    return m_positions.back().getTurnColor();
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
    return m_positions.back();
}

void GameData::setPosition(const Position& toSet)
{
    m_positions.push_back(toSet);
}

void GameData::setResult(Color winner)
{
    m_result = winner;
}

void GameData::newGame(Color whoUser, int time)
{
    plyCounter = 0;
    fiftyMovePlyCounter = 0;

    m_positions.clear();
    m_positions.emplace_back();

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
    m_positions.back().setPromotion(square, piece);
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
