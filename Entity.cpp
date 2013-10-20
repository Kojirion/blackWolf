#include "Entity.hpp"

thor::EventSystem<Message, std::string> Entity::messages;

Message::Message(const std::string &id):
    id(id)
{
}

Message::~Message()
{
}

MoveMessage::MoveMessage(const CompleteMove &move):
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
