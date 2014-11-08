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


NewGameMessage::NewGameMessage(Color whoUser, int time, std::string p1, std::string p2):
    Message("newGame"), user(whoUser), time(time), p1(p1), p2(p2)
{
}


EndGameMessage::EndGameMessage(Color result):
    Message("endGame"), result(result)
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
