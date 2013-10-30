#include "Application.hpp"

Application::Application():
    window(sf::VideoMode(1000, 1000), "Black Wolf")
{
    window.setFramerateLimit(60);

    desktop.LoadThemeFromFile("gui.theme");

    //icon.loadFromFile("Graphics/Boardbrown.jpg");
    //window.setIcon(32,32,icon.getPixelsPtr());

    actions["close"] = thor::Action(sf::Event::Closed);
    system.connect("close", std::bind(&sf::RenderWindow::close, &window));
}

void Application::run()
{
    Controller boss(window, desktop);

    window.resetGLStates();
    sf::Clock clock;

    while (window.isOpen())
    {
        actions.clearEvents();

        sf::Event event;
        while (window.pollEvent(event)){
          actions.pushEvent(event);
          desktop.HandleEvent(event);
        }

        actions.invokeCallbacks(system, &(window));

        desktop.Update(clock.restart().asSeconds());

        window.clear();
        boss.update();
        sfgui_.Display(window);
        window.display();
    }

}


