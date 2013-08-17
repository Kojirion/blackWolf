#ifndef SETTINGSSTATE_H
#define SETTINGSSTATE_H
#include <SFGUI/Window.hpp>
#include <SFGUI/Desktop.hpp>
#include <SFGUI/Button.hpp>
#include <SFML/System/NonCopyable.hpp>

class settingsState : private sf::NonCopyable
{
private:

public:
    sfg::Window::Ptr window;
    //sfg::Desktop& desktop;
    settingsState();
    sfg::Button::Ptr closeButton;
    void makeVisible();
};

#endif // SETTINGSSTATE_H
