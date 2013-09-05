#include "clocksview.h"

clocksView::clocksView():
    whiteClockCanvas(sfg::Canvas::Create()),
    blackClockCanvas(sfg::Canvas::Create()),
    whiteClockLabel(sfg::Label::Create()),
    blackClockLabel(sfg::Label::Create())
{

}

void clocksView::update(const sf::Time &whiteTime, const sf::Time &blackTime)
{
    whiteClockLabel->SetText(timeToString(whiteTime));
    blackClockLabel->SetText(timeToString(blackTime));
}

sfg::Widget::Ptr clocksView::getWhiteClock()
{
    return whiteClockLabel;
}

sfg::Widget::Ptr clocksView::getBlackClock()
{
    return blackClockLabel;
}

void clocksView::setFlagDown(const bw loser)
{

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
