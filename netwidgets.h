#ifndef NETWIDGETS_H
#define NETWIDGETS_H
#include <SFGUI/Window.hpp>
#include <SFGUI/ScrolledWindow.hpp>
#include <SFGUI/Box.hpp>
#include <SFGUI/Entry.hpp>

class netWidgets
{
public:
    netWidgets();

private:
    sfg::Window::Ptr window;
    sfg::Box::Ptr chatAreaLayout;
    sfg::Entry::Ptr chatEntry;
    sfg::ScrolledWindow::Ptr chatWindow;
};

#endif // NETWIDGETS_H
