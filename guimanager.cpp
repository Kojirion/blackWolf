#include "guimanager.h"

guiManager::guiManager():
    window(sf::VideoMode(600, 600), "Black Wolf")
{
}

void guiManager::run()
{
    boardMaster boss;

    sfg::Window::Ptr boardWindow(sfg::Window::Create());
    //boardWindow->SetTitle( "Board" );
    boardWindow->Add(boss.window_);

    sfg::Desktop desktop;
    desktop.Add(boardWindow);

    window.resetGLStates();
    sf::Clock clock;
    boss.display();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            desktop.HandleEvent(event);

            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed){
                //if event.mo
            }
        }

        desktop.Update(clock.restart().asSeconds());

        window.clear();

        sfgui_.Display(window);
        window.display();
    }

}
