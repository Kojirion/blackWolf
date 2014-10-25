#include "Application.hpp"

Application::Application():
    window(sf::VideoMode(800, 800), "Black Wolf")
{
    window.setFramerateLimit(60);

    desktop.LoadThemeFromFile("gui.theme");

    auto font = std::make_shared<sf::Font>(sf::Font());
    font->loadFromFile("DejaVuSansMono.ttf");
    desktop.GetEngine().GetResourceManager().SetDefaultFont(font);
    desktop.SetProperty( "Label#chatText", "FontName",  "DejaVuSansMono.ttf" );

    actions["close"] = thor::Action(sf::Event::Closed);
    system.connect("close", std::bind(&sf::RenderWindow::close, &window));

    actions["resized"] = thor::Action(sf::Event::Resized);
    system.connect("resized", std::bind(&MessageSystem::triggerEvent, &messages, ResizeMessage(window)));
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


