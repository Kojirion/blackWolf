#ifndef CLOCKSVIEW_H
#define CLOCKSVIEW_H
#include <SFGUI/Label.hpp>
#include "../BlackWolf.hpp"
#include <SFML/System/Time.hpp>

class Clocks
{
private:
    sfg::Label::Ptr whiteClockLabel;
    sfg::Label::Ptr blackClockLabel;

    std::string timeToString(const sf::Time& value) const;
public:
    Clocks();
    void update(const sf::Time& whiteTime, const sf::Time& blackTime);
    sfg::Widget::Ptr getWhiteClock();
    sfg::Widget::Ptr getBlackClock();
};

#endif // CLOCKSVIEW_H
