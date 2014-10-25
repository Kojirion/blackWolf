#pragma once
#include <SFML/Graphics.hpp>
#include <SFGUI/SFGUI.hpp>
#include "controller/Controller.hpp"
#include <Thor/Input/ActionMap.hpp>
#include "Entity.hpp"

class Application : private Entity
{
private:
    sfg::SFGUI sfgui_;
    sf::RenderWindow window;
    sfg::Desktop desktop;    
    thor::ActionMap<std::string> actions;
    thor::ActionMap<std::string>::CallbackSystem system;

public:
    Application();

    void run();
};
