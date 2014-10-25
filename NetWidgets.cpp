#include "NetWidgets.hpp"
#include <SFGUI/Label.hpp>
#include <SFGUI/Adjustment.hpp>

NetWidgets::NetWidgets():
    chatLayout(sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 5.f)),
    chatEntry(sfg::Entry::Create()),
    chatText(sfg::Label::Create()),
    chatWindow(sfg::ScrolledWindow::Create())
{
    chatText->SetId("chatText");

    chatWindow->SetRequisition(sf::Vector2f(600.f,600.f));
    chatWindow->SetScrollbarPolicy( sfg::ScrolledWindow::HORIZONTAL_AUTOMATIC | sfg::ScrolledWindow::VERTICAL_AUTOMATIC );
    chatWindow->AddWithViewport(chatText);

    chatLayout->Pack(chatWindow);
    chatLayout->Pack(chatEntry);

    chatEntry->GetSignal(sfg::Entry::OnKeyPress).Connect(std::bind(&NetWidgets::sendData, this));
    chatEntry->GrabFocus();
}

sfg::Widget::Ptr NetWidgets::getWidget()
{
    return chatLayout;
}

void NetWidgets::addLine(const std::string& line)
{
    chatText->SetText(chatText->GetText() + line);
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
