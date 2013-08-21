#include "buttonbox.h"

buttonBox::buttonBox():
    buttonLayout(sfg::Table::Create()),
    resignButton(sfg::Button::Create("Resign")),
    drawButton(sfg::Button::Create("Offer draw")),
    newGameButton(sfg::Button::Create("New game")),
    flipButton(sfg::Button::Create("Flip board"))
{
    buttonLayout->SetRowSpacings(3.f);
    buttonLayout->SetColumnSpacings(3.f);

    buttonLayout->Attach(resignButton,{0,0,1,1});
    buttonLayout->Attach(drawButton,{0,1,1,1});
    buttonLayout->Attach(newGameButton,{1,0,1,1});
    buttonLayout->Attach(flipButton,{1,1,1,1});
    //buttonLayout->Attach(settingsButton,{2,0,1,1});
}

sfg::Widget::Ptr buttonBox::getWidget()
{
    return buttonLayout;
}

sfg::Widget::Ptr buttonBox::resign()
{
    return resignButton;
}

sfg::Widget::Ptr buttonBox::draw()
{
    return drawButton;
}

sfg::Widget::Ptr buttonBox::newGame()
{
    return newGameButton;
}

sfg::Widget::Ptr buttonBox::flip()
{
    return flipButton;
}
