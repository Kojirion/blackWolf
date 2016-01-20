#include "NetWidgets.hpp"
#include <SFGUI/Label.hpp>
#include <SFGUI/Adjustment.hpp>

NetWidgets::NetWidgets():
    m_chatLayout(sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 5.f)),
    m_chatEntry(sfg::Entry::Create()),
    m_chatText(sfg::Label::Create()),
    m_chatWindow(sfg::ScrolledWindow::Create())
{
    m_chatText->SetId("chatText");

    m_chatWindow->SetRequisition(sf::Vector2f(600.f,700.f));
    m_chatWindow->SetScrollbarPolicy( sfg::ScrolledWindow::HORIZONTAL_AUTOMATIC | sfg::ScrolledWindow::VERTICAL_AUTOMATIC );
    m_chatWindow->AddWithViewport(m_chatText);

    m_chatLayout->Pack(m_chatWindow);
    m_chatLayout->Pack(m_chatEntry);

    m_chatEntry->GetSignal(sfg::Entry::OnKeyPress).Connect(std::bind(&NetWidgets::sendData, this));
    m_chatEntry->GrabFocus();

    auto adjustment = m_chatWindow->GetVerticalAdjustment();
    adjustment->GetSignal(sfg::Adjustment::OnChange).Connect(
                               std::bind(&sf::Clock::restart, m_clock));
}

sfg::Widget::Ptr NetWidgets::getWidget()
{
    return m_chatLayout;
}

void NetWidgets::addLine(const std::string& line)
{
    m_chatText->SetText(m_chatText->GetText() + line);
    autoscroll();
}

void NetWidgets::scroll(int delta)
{
    if (!m_chatWindow->IsGloballyVisible())
        return;
    auto adjustment = m_chatWindow->GetVerticalAdjustment();
    adjustment->SetMinorStep(25.f);
    if (delta<0){
        adjustment->Increment();
        m_clock.restart();
    }else{
        adjustment->Decrement();
        m_clock.restart();
    }
}

void NetWidgets::autoscroll()
{
    auto timeOfNoScroll = m_clock.getElapsedTime().asSeconds();
    if (timeOfNoScroll>5.f){
        sfg::Adjustment::Ptr toAdjust(m_chatWindow->GetVerticalAdjustment());
        toAdjust->SetValue(toAdjust->GetUpper());
    }
}

void NetWidgets::sendData()
{
    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) return;
    std::string toWrite = m_chatEntry->GetText();
    m_chatEntry->SetText("");
    sendText(toWrite);
}
