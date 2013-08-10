#ifndef GUIMANAGER_H
#define GUIMANAGER_H
#include <SFML/Graphics.hpp>
#include <SFGUI/SFGUI.hpp>
#include "boardmaster.h"

class guiManager
{
private:
    sfg::SFGUI sfgui_;
    sf::RenderWindow window;

public:
    guiManager();

    void run();

};

#endif // GUIMANAGER_H
