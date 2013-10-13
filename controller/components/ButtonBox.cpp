#include "ButtonBox.hpp"

ButtonBox::ButtonBox():
    buttonLayout(sfg::Table::Create()),
    resignButton(sfg::Button::Create("Resign")),
    drawButton(sfg::Button::Create("Offer draw")),
    newGameButton(sfg::Button::Create("New game")),
    flipButton(sfg::Button::Create("Flip board")),
    settingsButton(sfg::Button::Create("Settings"))
{
    buttonLayout->SetRowSpacings(3.f);
    buttonLayout->SetColumnSpacings(3.f);

    buttonLayout->Attach(resignButton,{0,0,1,1});
    buttonLayout->Attach(drawButton,{0,1,1,1});
    buttonLayout->Attach(newGameButton,{1,0,1,1});
    buttonLayout->Attach(flipButton,{1,1,1,1});
    buttonLayout->Attach(settingsButton,{2,0,1,1});
}

sfg::Widget::Ptr ButtonBox::getWidget()
{
    return buttonLayout;
}

sfg::Widget::Ptr ButtonBox::resign()
{
    return resignButton;
}

sfg::Widget::Ptr ButtonBox::draw()
{
    return drawButton;
}

sfg::Widget::Ptr ButtonBox::newGame()
{
    return newGameButton;
}

sfg::Widget::Ptr ButtonBox::flip()
{
    return flipButton;
}

sfg::Widget::Ptr ButtonBox::settings()
{
    return settingsButton;
}
