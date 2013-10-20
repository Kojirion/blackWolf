#ifndef ENTITY_HPP
#define ENTITY_HPP
#include <Thor/Input.hpp>
#include "common/CompleteMove.hpp"

struct Message{
    Message(const std::string& id):
      id(id) {}

    virtual ~Message();

    std::string id;
};

struct MoveMessage : public Message{
    MoveMessage(const std::string& id, const CompleteMove& move);

    CompleteMove move;
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
