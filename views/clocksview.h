#ifndef CLOCKSVIEW_H
#define CLOCKSVIEW_H
#include <SFGUI/Label.hpp>
#include <SFGUI/Canvas.hpp>
#include "blackWolf.h"

class clocksView
{
private:
    sfg::Label::Ptr whiteClockLabel;
    sfg::Label::Ptr blackClockLabel;

    sfg::Canvas::Ptr whiteClockCanvas;
    sfg::Canvas::Ptr blackClockCanvas;

    sf::Font font;

    sf::Text whiteClockText;
    sf::Text blackClockText;

    std::string timeToString(const sf::Time& value) const;
public:
    clocksView();
    void update(const sf::Time& whiteTime, const sf::Time& blackTime);
    sfg::Widget::Ptr getWhiteClock();
    sfg::Widget::Ptr getBlackClock();

    void setFlagDown(const bw loser);
};

#endif // CLOCKSVIEW_H
