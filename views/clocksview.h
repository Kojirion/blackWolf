#ifndef CLOCKSVIEW_H
#define CLOCKSVIEW_H
#include <SFGUI/Canvas.hpp>

class clocksView
{
private:
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
};

#endif // CLOCKSVIEW_H
