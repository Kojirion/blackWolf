#pragma once
#include <Thor/Input.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "Message.hpp"

namespace Messages{

using System = thor::EventSystem<Message, std::string>;

class Registrant
{

protected:
    static System messages;
};

}
