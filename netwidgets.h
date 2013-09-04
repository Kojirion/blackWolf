#ifndef NETWIDGETS_H
#define NETWIDGETS_H
#include <SFGUI/ScrolledWindow.hpp>
#include <SFGUI/Box.hpp>
#include <SFGUI/Entry.hpp>
#include <boost/signals2.hpp>

class netWidgets
{
public:
    netWidgets();

    sfg::Widget::Ptr getWidget();

    void addLine(std::string line);

    boost::signals2::signal<void (std::string)> sendText;

private:
    sfg::Box::Ptr chatLayout;
    sfg::Box::Ptr chatAreaLayout;
    sfg::Entry::Ptr chatEntry;
    sfg::ScrolledWindow::Ptr chatWindow;

    void autoscroll();

    void sendData();
};

#endif // NETWIDGETS_H
