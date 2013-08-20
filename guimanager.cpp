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
    //connect settings close to button to signal
    //guiManager's involment could be done away with
//    stateManager.settingsData.closeButton->
//            GetSignal(sfg::Button::OnLeftClick).Connect(&guiManager::settingsClosed, this);

    boardMaster boss(window, desktop);

//    stateManager.boardData.settingsButton->GetSignal(sfg::Button::OnLeftClick).Connect(&guiManager::settingsClicked, this);

    window.resetGLStates();
    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            boss.desktop.HandleEvent(event);

            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed){
                //if event.mo
            }
        }

        boss.desktop.Update(clock.restart().asSeconds());

        window.clear();
        boss.display();
        sfgui_.Display(window);
        window.display();
    }

}


