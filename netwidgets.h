#ifndef NETWIDGETS_H
#define NETWIDGETS_H
#include <SFGUI/ScrolledWindow.hpp>
#include <SFGUI/Box.hpp>
#include <SFGUI/Entry.hpp>

class netWidgets
{
public:
    netWidgets();

    sfg::Widget::Ptr getWidget();

    void addLine(std::string line);

private:
    sfg::Box::Ptr chatLayout;
    sfg::Box::Ptr chatAreaLayout;
    sfg::Entry::Ptr chatEntry;
    sfg::ScrolledWindow::Ptr chatWindow;

    void autoscroll();
};

#endif // NETWIDGETS_H
