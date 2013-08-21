#include "clocksview.h"

clocksView::clocksView():
    whiteClockCanvas(sfg::Canvas::Create()),
    blackClockCanvas(sfg::Canvas::Create())
{
    font.loadFromFile("DejaVuSans.ttf"); //assert it

    whiteClockText.setFont(font);
    whiteClockText.setCharacterSize(30);
    whiteClockText.setPosition(22.f,8.f);
    whiteClockText.setColor(sf::Color(0, 140, 190));

    blackClockText.setFont(font);
    blackClockText.setCharacterSize(30);
    blackClockText.setPosition(22.f, 8.f);
    blackClockText.setColor(sf::Color(0, 140, 190));

    whiteClockCanvas->SetRequisition(sf::Vector2f(100,50));
    blackClockCanvas->SetRequisition(sf::Vector2f(100,50));

}

void clocksView::update(const sf::Time &whiteTime, const sf::Time &blackTime)
{
    whiteClockText.setString(timeToString(whiteTime));
    whiteClockCanvas->Clear();
    whiteClockCanvas->Draw(whiteClockText);

    blackClockText.setString(timeToString(blackTime));
    blackClockCanvas->Clear();
    blackClockCanvas->Draw(blackClockText);
}

sfg::Widget::Ptr clocksView::getWhiteClock()
{
    return whiteClockCanvas;
}

sfg::Widget::Ptr clocksView::getBlackClock()
{
    return blackClockCanvas;
}

void clocksView::setFlagDown(const bw loser)
{
    if (loser == bw::White) whiteClockText.setColor(sf::Color::Yellow);
    else blackClockText.setColor(sf::Color::Yellow);
}

std::string clocksView::timeToString(const sf::Time& value) const
{
    const int totalSeconds = static_cast<int>(std::ceil(value.asSeconds()));
    int minutes = totalSeconds/60;
    int seconds = totalSeconds%60;

    std::string toReturn = std::to_string(minutes) + ":";
    if (seconds<10) toReturn += "0" + std::to_string(seconds);
    else toReturn += std::to_string(seconds);

    return toReturn;
}
