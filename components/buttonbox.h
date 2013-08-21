#ifndef BUTTONBOX_H
#define BUTTONBOX_H
#include <SFGUI/Table.hpp>
#include <SFGUI/Button.hpp>

class buttonBox
{
private:
    sfg::Table::Ptr buttonLayout;

    sfg::Button::Ptr resignButton;
    sfg::Button::Ptr drawButton;
    sfg::Button::Ptr newGameButton;
    sfg::Button::Ptr flipButton;
public:
    buttonBox();

    sfg::Widget::Ptr getWidget();

    sfg::Widget::Ptr resign();
    sfg::Widget::Ptr draw();
    sfg::Widget::Ptr newGame();
    sfg::Widget::Ptr flip();


};

#endif // BUTTONBOX_H
