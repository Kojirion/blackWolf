#include "Entity.hpp"

thor::EventSystem<Message, std::string> Entity::messages;

Message::Message(const std::string &id):
    id(id)
{
}

Message::~Message()
{
}

std::string getEventId(const Message &message)
{
    return message.id;
}

Entity::Entity()
{
}


NewGameMessage::NewGameMessage():
    Message("newGame")
{
}


EndGameMessage::EndGameMessage():
    Message("endGame")
{
}


GameStateMessage::GameStateMessage():Message("gameState") { }
