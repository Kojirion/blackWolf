#pragma once
#include <Thor/Input.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "Message.hpp"

using MessageSystem = thor::EventSystem<Message, std::string>;

class Entity
{

protected:
    static MessageSystem messages;
};
