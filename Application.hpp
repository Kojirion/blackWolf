#pragma once
#include <SFML/Graphics.hpp>
#include <SFGUI/SFGUI.hpp>
#include "controller/Controller.hpp"

#include "messages/Entity.hpp"
#include "Actions.hpp"

class Application : private Messages::Registrant
{
public:
    Application();
    void run();

private:
    sf::RenderWindow m_window;
    sfg::SFGUI m_sfgui;    
    sfg::Desktop m_desktop;
    Actions m_actions;
    CallbackSystem m_system;
};
