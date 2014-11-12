#pragma once
#include <Thor/Input.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "BlackWolf.hpp"
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/optional.hpp>

struct Message{
    Message(const std::string& id);

    virtual ~Message();

    std::string id;
};

typedef thor::EventSystem<Message, std::string> MessageSystem;

struct MoveMessage : public Message {
    MoveMessage(const std::string& move);

    std::string move;
};

struct NewGameMessage : public Message {
    NewGameMessage();

    std::string p1;
    std::string p2;
    int time;
    Color user;
};

BOOST_FUSION_ADAPT_STRUCT(
        NewGameMessage,
        (std::string, p1)
        (std::string, p2)
        (int, time)
        )

using ParsedPosition = std::vector<std::vector<Unit>>;

struct GameStateMessage : public Message {
    GameStateMessage();
    ParsedPosition position;
    Color turnColor;
    int white_time, black_time;
    boost::optional<Square> target_square;
    std::string move;
};

BOOST_FUSION_ADAPT_STRUCT(
        GameStateMessage,
        (ParsedPosition, position)
        (Color, turnColor)
        (int, white_time)
        (int, black_time)
        (boost::optional<Square>, target_square)
        (std::string, move)
        )

struct EndGameMessage : public Message {
    EndGameMessage();

    Color result;
};

BOOST_FUSION_ADAPT_STRUCT(
        EndGameMessage,
        (Color, result)
        )

struct ResizeMessage : public Message {
    ResizeMessage(const sf::RenderWindow& window);

    const sf::RenderWindow& window;
};

struct CountMessage : public Message {
    CountMessage(unsigned int repetitions, unsigned int fiftyMoves);

    unsigned int repetitions;
    unsigned int fiftyMoves;
};



std::string getEventId(const Message &message);



class Entity
{
public:
    Entity();

protected:
    static MessageSystem messages;
};
