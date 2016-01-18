#pragma once
#include <SFML/Graphics.hpp>
#include <SFGUI/SFGUI.hpp>
#include "controller/Controller.hpp"
#include <Thor/Input/ActionMap.hpp>
#include "Entity.hpp"

class Application : private Entity
{
public:
    Application();
    void run();

private:    
    enum class Action{
        Close,
        Resize
    };

    sfg::SFGUI m_sfgui;
    sf::RenderWindow m_window;
    sfg::Desktop m_desktop;

    using Actions = thor::ActionMap<Action>;
    Actions m_actions;
    Actions::CallbackSystem m_system;
};
