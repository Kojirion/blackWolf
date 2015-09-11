#include "Clocks.hpp"

Clocks::Clocks():    
    whiteClockLabel(sfg::Label::Create()),
    blackClockLabel(sfg::Label::Create())
{

}

void Clocks::update(const sf::Time &whiteTime, const sf::Time &blackTime)
{
    whiteClockLabel->SetText(timeToString(whiteTime));
    blackClockLabel->SetText(timeToString(blackTime));
}

sfg::Widget::Ptr Clocks::getWhiteClock()
{
    return whiteClockLabel;
}

sfg::Widget::Ptr Clocks::getBlackClock()
{
    return blackClockLabel;
}

std::string Clocks::timeToString(const sf::Time& value) const
{
    const int totalSeconds = static_cast<int>(std::ceil(value.asSeconds()));
    int minutes = totalSeconds/60;
    int seconds = totalSeconds%60;

    std::string toReturn = std::to_string(minutes) + ":";
    if (seconds<10) toReturn += "0" + std::to_string(seconds);
    else toReturn += std::to_string(seconds);

    return toReturn;
}
