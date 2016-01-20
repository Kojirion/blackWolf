#include "ButtonBox.hpp"

ButtonBox::ButtonBox():
    layout(sfg::Table::Create()),
    resign(sfg::Button::Create("Resign")),
    draw(sfg::Button::Create("Offer draw")),
    newGame(sfg::Button::Create("New game")),
    flip(sfg::Button::Create("Flip board")),
    settings(sfg::Button::Create("Settings")),
    connect(sfg::Button::Create("Connect"))
{
    layout->SetRowSpacings(3.f);
    layout->SetColumnSpacings(3.f);

    layout->Attach(resign,{0,0,1,1});
    layout->Attach(draw,{0,1,1,1});
    layout->Attach(newGame,{1,0,1,1});
    layout->Attach(flip,{1,1,1,1});
    layout->Attach(settings,{2,0,1,1});
    layout->Attach(connect,{2,1,1,1});
}
