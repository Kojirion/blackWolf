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

    void scroll(int delta);

    boost::signals2::signal<void (std::string)> sendText;

private:
    sfg::Box::Ptr m_chatLayout;
    sfg::Entry::Ptr m_chatEntry;
    sfg::Label::Ptr m_chatText;
    sfg::ScrolledWindow::Ptr m_chatWindow;

    void autoscroll();

    void sendData();    
};
