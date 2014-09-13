#pragma once
#include <SFGUI/Table.hpp>
#include <SFGUI/Button.hpp>

class ButtonBox
{
private:
    sfg::Table::Ptr buttonLayout;

    sfg::Button::Ptr resignButton;
    sfg::Button::Ptr drawButton;
    sfg::Button::Ptr newGameButton;
    sfg::Button::Ptr flipButton;
    sfg::Button::Ptr settingsButton;
public:
    ButtonBox();

    sfg::Widget::Ptr getWidget();

    sfg::Widget::Ptr resign();
    sfg::Widget::Ptr draw();
    sfg::Widget::Ptr newGame();
    sfg::Widget::Ptr flip();
    sfg::Widget::Ptr settings();


};
