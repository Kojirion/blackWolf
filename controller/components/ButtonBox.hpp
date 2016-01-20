#pragma once
#include <SFGUI/Table.hpp>
#include <SFGUI/Button.hpp>

struct ButtonBox
{
    ButtonBox();

    sfg::Table::Ptr layout;
    sfg::Button::Ptr resign, draw, newGame, flip, settings, connect;

};
