#include "Entity.hpp"

thor::EventSystem<Message, std::string> Entity::messages;

Entity::Entity()
{
}

std::string getEventId(const Message &message)
{
    return message.id;
}


Message::~Message()
{
}
