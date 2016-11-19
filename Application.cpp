#include "Application.hpp"
#include <SFGUI/Renderers/VertexArrayRenderer.hpp>


Application::Application():
    m_window(sf::VideoMode(800, 800), "Black Wolf", sf::Style::Close)
{
    sfg::Renderer::Set( sfg::VertexArrayRenderer::Create() );

    m_window.setFramerateLimit(60);

    auto font = std::make_shared<sf::Font>(sf::Font());
    font->loadFromFile("DejaVuSansMono.ttf");
    m_desktop.GetEngine().GetResourceManager().SetDefaultFont(font);
    m_desktop.SetProperty( "Label#chatText", "FontName",  "DejaVuSansMono.ttf" );

    m_actions[Action::Close] = thor::Action(sf::Event::Closed);
    m_system.connect(Action::Close, std::bind(&sf::RenderWindow::close, &m_window));

    m_actions[Action::Scroll] = thor::Action(sf::Event::MouseWheelScrolled);
    m_actions[Action::Tab] = thor::Action(sf::Keyboard::Tab, thor::Action::ActionType::PressOnce);
}

void Application::run()
{
    Controller controller(m_window, m_desktop, m_system);

    m_window.resetGLStates();
    sf::Clock clock;

    while (m_window.isOpen())
    {
        m_actions.clearEvents();

        sf::Event event;
        while (m_window.pollEvent(event)){
            controller.setEvent(event);
            m_actions.pushEvent(event);
            m_desktop.HandleEvent(event);
        }

        m_actions.invokeCallbacks(m_system, &(m_window));

        m_desktop.Update(clock.restart().asSeconds());

        m_window.clear();
        controller.update();
        m_sfgui.Display(m_window);
        m_window.display();
    }
}


