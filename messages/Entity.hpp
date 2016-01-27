#pragma once
#include <Thor/Input.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "Message.hpp"

namespace Messages{

using System = thor::EventSystem<Message, ID>;

class Registrant
{

protected:
    static System messages;
};

}
