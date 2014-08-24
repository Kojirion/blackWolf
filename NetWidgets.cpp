#include "NetWidgets.hpp"
#include <SFGUI/Label.hpp>
#include <SFGUI/Adjustment.hpp>

NetWidgets::NetWidgets():
    chatLayout(sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 5.f)),
    chatAreaLayout(sfg::Box::Create(sfg::Box::Orientation::VERTICAL)),
    chatEntry(sfg::Entry::Create()),
    chatWindow(sfg::ScrolledWindow::Create())
{
    chatWindow->SetRequisition(sf::Vector2f(600.f,600.f));
    chatWindow->SetScrollbarPolicy( sfg::ScrolledWindow::HORIZONTAL_AUTOMATIC | sfg::ScrolledWindow::VERTICAL_AUTOMATIC );
    chatWindow->AddWithViewport(chatAreaLayout);

    chatLayout->Pack(chatWindow);
    chatLayout->Pack(chatEntry);

    chatEntry->GetSignal(sfg::Entry::OnKeyPress).Connect(std::bind(&NetWidgets::sendData, this));
    chatEntry->GrabFocus();

    //chatWindow->GetSignal(sfg::Adjustment::OnChange).Connect(&netWidgets::autoscroll, this);

    //chatEntry->GetSignal(sfg::Entry::OnKeyPress)
}

sfg::Widget::Ptr NetWidgets::getWidget()
{
    return chatLayout;
}

void NetWidgets::addLine(std::string line)
{
    sfg::Container::WidgetsList lines(chatAreaLayout->GetChildren());
    if (lines.size()>120) //if more, pop front
        chatAreaLayout->Remove(lines.front());


    sfg::Label::Ptr toAdd(sfg::Label::Create(line));
    toAdd->SetAlignment(sf::Vector2f(0.f,0.f));
    chatAreaLayout->PackEnd(toAdd);
    autoscroll();
}

void NetWidgets::autoscroll()
{
    sfg::Adjustment::Ptr toAdjust(chatWindow->GetVerticalAdjustment());
    toAdjust->SetValue(toAdjust->GetUpper());
}

void NetWidgets::sendData()
{
    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) return;
    std::string toWrite = chatEntry->GetText();
    chatEntry->SetText("");
    sendText(toWrite);
}
