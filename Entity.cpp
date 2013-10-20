#include "Entity.hpp"

thor::EventSystem<Message, std::string> Entity::messages;

Message::~Message()
{
}

MoveMessage::MoveMessage(const std::string &id, const CompleteMove &move):
    Message(id), move(move)
{

}

std::string getEventId(const Message &message)
{
    return message.id;
}

Entity::Entity()
{
}
