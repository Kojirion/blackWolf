#include "Message.hpp"

Message::Message(const std::string &id):
    id(id)
{
}

std::string getEventId(const Message &message)
{
    return message.id;
}
