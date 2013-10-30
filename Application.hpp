#ifndef GUIMANAGER_H
#define GUIMANAGER_H
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
    //sf::Image icon;
    thor::ActionMap<std::string> actions;
    thor::ActionMap<std::string>::CallbackSystem system;

public:
    Application();

    void run();
};

#endif // GUIMANAGER_H
