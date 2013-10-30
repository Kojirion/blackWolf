#ifndef ENTITY_HPP
#define ENTITY_HPP
#include <Thor/Input.hpp>
#include "common/CompleteMove.hpp"
#include <SFML/Graphics/RenderWindow.hpp>

struct Message{
    Message(const std::string& id);

    virtual ~Message();

    std::string id;
};

typedef thor::EventSystem<Message, std::string> MessageSystem;

struct MoveMessage : public Message {
    MoveMessage(const CompleteMove& move);

    CompleteMove move;
};

struct NewGameMessage : public Message {
    NewGameMessage(Color whoUser, int time, std::string p1, std::string p2);

    Color user;
    int time;
    std::string p1;
    std::string p2;
};

struct EndGameMessage : public Message {
    EndGameMessage(Color result);

    Color result;
};

struct ResizeMessage : public Message {
    ResizeMessage(const sf::RenderWindow& window);

    const sf::RenderWindow& window;
};



std::string getEventId(const Message &message);



class Entity
{
public:
    Entity();

protected:
    static MessageSystem messages;
};

#endif // ENTITY_HPP
