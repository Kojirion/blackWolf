#ifndef ENTITY_HPP
#define ENTITY_HPP
#include <Thor/Input.hpp>
#include "common/CompleteMove.hpp"

struct Message{
    Message(const std::string& id);

    virtual ~Message();

    std::string id;
};

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



std::string getEventId(const Message &message);



class Entity
{
public:
    Entity();

protected:
    static thor::EventSystem<Message, std::string> messages;
};

#endif // ENTITY_HPP
