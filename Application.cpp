#include "Application.hpp"

Application::Application():
    m_window(sf::VideoMode(800, 800), "Black Wolf")
{
    m_window.setFramerateLimit(60);
    m_desktop.LoadThemeFromFile("gui.theme");

    auto font = std::make_shared<sf::Font>(sf::Font());
    font->loadFromFile("DejaVuSansMono.ttf");
    m_desktop.GetEngine().GetResourceManager().SetDefaultFont(font);
    m_desktop.SetProperty( "Label#chatText", "FontName",  "DejaVuSansMono.ttf" );

    m_actions[Action::Close] = thor::Action(sf::Event::Closed);
    m_system.connect(Action::Close, std::bind(&sf::RenderWindow::close, &m_window));

    m_actions[Action::Scroll] = thor::Action(sf::Event::MouseWheelScrolled);

    m_actions[Action::Resize] = thor::Action(sf::Event::Resized);
    m_system.connect(Action::Resize, std::bind(&MessageSystem::triggerEvent, &messages, ResizeMessage(m_window)));
}

void Application::run()
{
    Controller boss(m_window, m_desktop, m_system);

    m_window.resetGLStates();
    sf::Clock clock;

    while (m_window.isOpen())
    {
        m_actions.clearEvents();

        sf::Event event;
        while (m_window.pollEvent(event)){
          m_actions.pushEvent(event);
          m_desktop.HandleEvent(event);
        }

        m_actions.invokeCallbacks(m_system, &(m_window));

        m_desktop.Update(clock.restart().asSeconds());

        m_window.clear();
        boss.update();
        m_sfgui.Display(m_window);
        m_window.display();
    }

}


