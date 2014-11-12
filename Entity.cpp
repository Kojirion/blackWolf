#include "Entity.hpp"

thor::EventSystem<Message, std::string> Entity::messages;

Message::Message(const std::string &id):
    id(id)
{
}

Message::~Message()
{
}

MoveMessage::MoveMessage(const std::string &move):
    Message("moveMade"), move(move)
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


ResizeMessage::ResizeMessage(const sf::RenderWindow &window):
    Message("resized"), window(window)
{
}


CountMessage::CountMessage(unsigned int repetitions, unsigned int fiftyMoves):
    Message("count"), repetitions(repetitions), fiftyMoves(fiftyMoves)
{
}


GameStateMessage::GameStateMessage():Message("gameState") { }
