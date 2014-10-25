#pragma once
#include <SFGUI/ScrolledWindow.hpp>
#include <SFGUI/Box.hpp>
#include <SFGUI/Entry.hpp>
#include <SFGUI/Label.hpp>
#include <boost/signals2.hpp>

class NetWidgets
{
public:
    NetWidgets();

    sfg::Widget::Ptr getWidget();

    void addLine(const std::string &line);

    boost::signals2::signal<void (std::string)> sendText;

private:
    sfg::Box::Ptr chatLayout;
    sfg::Entry::Ptr chatEntry;
    sfg::Label::Ptr chatText;
    sfg::ScrolledWindow::Ptr chatWindow;

    void autoscroll();

    void sendData();    
};
