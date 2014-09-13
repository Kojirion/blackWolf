#pragma once
#include <SFGUI/Window.hpp>
#include <SFGUI/Button.hpp>
#include <SFGUI/Desktop.hpp>

class ColorChoices
{
private:
    sfg::Window::Ptr window;
    sfg::Desktop& desktop;

    sfg::Button::Ptr whiteSide;
    sfg::Button::Ptr blackSide;
    sfg::Button::Ptr bothSide;

public:
    ColorChoices(sfg::Desktop& theDesktop);

    void enable(bool doEnable = true);

    sfg::Widget::Ptr getWidget();

    sfg::Button::Ptr getWhiteSide();
    sfg::Button::Ptr getBlackSide();
    sfg::Button::Ptr getBothSide();


};
