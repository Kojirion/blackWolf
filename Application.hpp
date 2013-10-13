#ifndef GUIMANAGER_H
#define GUIMANAGER_H
#include <SFML/Graphics.hpp>
#include <SFGUI/SFGUI.hpp>
#include "controller/BoardMaster.hpp"

class Application
{
private:
    sfg::SFGUI sfgui_;
    sf::RenderWindow window;
    sfg::Desktop desktop;    
    //sf::Image icon;

public:
    Application();

    void run();
};

#endif // GUIMANAGER_H
