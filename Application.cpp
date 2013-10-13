#include "Application.hpp"

Application::Application():
    window(sf::VideoMode(634, 694), "Black Wolf")
{
    window.setFramerateLimit(60);

    desktop.LoadThemeFromFile("gui.theme");

    //icon.loadFromFile("Graphics/Boardbrown.jpg");
    //window.setIcon(32,32,icon.getPixelsPtr());
}

void Application::run()
{
    BoardMaster boss(window, desktop);

    window.resetGLStates();
    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            desktop.HandleEvent(event);

            if (event.type == sf::Event::Closed)
                window.close();            
        }

        desktop.Update(clock.restart().asSeconds());

        window.clear();
        boss.display();
        sfgui_.Display(window);
        window.display();
    }

}


