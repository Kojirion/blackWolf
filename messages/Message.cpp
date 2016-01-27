#include "Message.hpp"

namespace Messages{

Message::Message(ID id):
    id(id)
{
}

ID getEventId(const Message &message)
{
    return message.id;
}

}
