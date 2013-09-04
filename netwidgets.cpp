#include "netwidgets.h"
#include <SFGUI/Label.hpp>

netWidgets::netWidgets():
    chatLayout(sfg::Box::Create(sfg::Box::VERTICAL, 5.f)),
    chatAreaLayout(sfg::Box::Create(sfg::Box::VERTICAL)),
    chatEntry(sfg::Entry::Create()),
    chatWindow(sfg::ScrolledWindow::Create())
{
    chatWindow->SetRequisition(sf::Vector2f(600.f,600.f));
    chatWindow->SetScrollbarPolicy( sfg::ScrolledWindow::HORIZONTAL_AUTOMATIC | sfg::ScrolledWindow::VERTICAL_AUTOMATIC );
    chatWindow->AddWithViewport(chatAreaLayout);

    chatLayout->Pack(chatWindow);
    chatLayout->Pack(chatEntry);

    chatEntry->GetSignal(sfg::Entry::OnKeyPress).Connect(&netWidgets::sendData, this);

    //chatWindow->GetSignal(sfg::Adjustment::OnChange).Connect(&netWidgets::autoscroll, this);

    //chatEntry->GetSignal(sfg::Entry::OnKeyPress)
}

sfg::Widget::Ptr netWidgets::getWidget()
{
    return chatLayout;
}

void netWidgets::addLine(std::string line)
{
    sfg::Label::Ptr toAdd(sfg::Label::Create(line));
    toAdd->SetAlignment(sf::Vector2f(0.f,0.f));
    chatAreaLayout->PackEnd(toAdd);
    autoscroll();
}

void netWidgets::autoscroll()
{
    sfg::Adjustment::Ptr toAdjust(chatWindow->GetVerticalAdjustment());
    toAdjust->SetValue(toAdjust->GetUpper());
}

void netWidgets::sendData()
{
    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) return;
    std::string toWrite = chatEntry->GetText();
    chatEntry->SetText("");
    sendText(toWrite);
}
