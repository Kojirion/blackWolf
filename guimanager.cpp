#include "guimanager.h"

guiManager::guiManager():
    window(sf::VideoMode(600, 600), "Black Wolf")
{
}

void guiManager::run()
{
    boardMaster boss(window);

    sfg::Table::Ptr mainLayout(sfg::Table::Create());
    mainLayout->Attach(boss.window_,sf::Rect<sf::Uint32>( 0, 0, 1, 8 ),sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 10.f, 0.f ));
    mainLayout->Attach(boss.whiteClockCanvas_,sf::Rect<sf::Uint32>( 1, 0, 1, 1 ));
    mainLayout->Attach(boss.blackClockCanvas_,sf::Rect<sf::Uint32>( 1, 2, 1, 1 ));
    mainLayout->Attach(boss.turnLabel_,sf::Rect<sf::Uint32>( 1, 3, 1, 1 ));


    sfg::Window::Ptr boardWindow(sfg::Window::Create());
    //boardWindow->SetTitle( "Board" );
    boardWindow->Add(mainLayout);

    sfg::Desktop desktop;
    desktop.Add(boardWindow);

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
            if (event.type == sf::Event::MouseButtonPressed){
                //if event.mo
            }
        }

        desktop.Update(clock.restart().asSeconds());

        window.clear();
        boss.display();
        sfgui_.Display(window);
        window.display();
    }

}
