#ifndef GUIMANAGER_H
#define GUIMANAGER_H
#include <SFML/Graphics.hpp>
#include <SFGUI/SFGUI.hpp>
#include "boardmaster.h"
#include "msmmanager.h"

class guiManager
{
private:
    sfg::SFGUI sfgui_;
    sf::RenderWindow window;
    sfg::Desktop desktop;
    stateMachine stateManager;
    //sf::Image icon;

    void settingsClicked();
    void settingsClosed();
public:
    guiManager();

    void run();
};

#endif // GUIMANAGER_H
