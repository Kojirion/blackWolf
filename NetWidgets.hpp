#pragma once
#include <SFGUI/ScrolledWindow.hpp>
#include <SFGUI/Box.hpp>
#include <SFGUI/Entry.hpp>
#include <SFGUI/Label.hpp>
#include <SFML/System/Clock.hpp>
#include "messages/Entity.hpp"

class NetWidgets : private Messages::Registrant
{
public:
    NetWidgets(const std::reference_wrapper<const sf::Event> currentEvent);
    sfg::Widget::Ptr getWidget();
    void addLine(const std::string &line);
    void scroll(int delta);
    void grabEntryFocus();

private:
    sfg::Box::Ptr m_chatLayout;
    sfg::Entry::Ptr m_chatEntry;
    sfg::Label::Ptr m_chatText;
    sfg::ScrolledWindow::Ptr m_chatWindow;
    sf::Clock m_clock;

    using History = std::vector<std::string>;

    History m_history;
    History::const_reverse_iterator m_backHistory;
    History::const_iterator m_forwardHistory;


    void autoscroll();

    void entryKeyPressed(const sf::Event &event);
};
