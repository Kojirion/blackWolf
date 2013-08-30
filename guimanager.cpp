#include "guimanager.h"

void guiManager::settingsClicked()
{
    //stateManager.process_event(settingsOpen());
}

void guiManager::settingsClosed()
{
    //stateManager.process_event(settingsClose());
}

guiManager::guiManager():
    window(sf::VideoMode(600, 600), "Black Wolf")
    //stateManager(boost::ref(desktop), boost::ref(window))
{
    window.setFramerateLimit(60);
    //stateManager.start();

    //icon.loadFromFile("Graphics/Boardbrown.jpg");
    //window.setIcon(32,32,icon.getPixelsPtr());
}

void guiManager::run()
{
    boardMaster boss(window, desktop);

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


