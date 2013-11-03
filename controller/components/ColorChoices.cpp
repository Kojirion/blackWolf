#include "ColorChoices.hpp"
#include <SFGUI/Box.hpp>

ColorChoices::ColorChoices(sfg::Desktop &theDesktop):    
    window(sfg::Window::Create()),
    desktop(theDesktop),
    whiteSide(sfg::Button::Create("White")),
    blackSide(sfg::Button::Create("Black")),
    bothSide(sfg::Button::Create("Both"))
{
    window->SetPosition(sf::Vector2f(200.f,200.f));
    window->SetRequisition(sf::Vector2f(200.f,200.f));
    window->SetTitle("Choose color");

    whiteSide->SetId("whiteNewGame");
    blackSide->SetId("blackNewGame");
    bothSide->SetId("bothNewGame");

    sfg::Box::Ptr sideChoiceBox(sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 5.f));

    sideChoiceBox->Pack(whiteSide);
    sideChoiceBox->Pack(blackSide);
    sideChoiceBox->Pack(bothSide);
    window->Add(sideChoiceBox);

    enable(false);
}

void ColorChoices::enable(bool doEnable)
{
    if (doEnable) desktop.BringToFront(window);
    window->Show(doEnable);
}

sfg::Widget::Ptr ColorChoices::getWidget()
{
    return window;
}

sfg::Button::Ptr ColorChoices::getWhiteSide()
{
    return whiteSide;
}

sfg::Button::Ptr ColorChoices::getBlackSide()
{
    return blackSide;
}

sfg::Button::Ptr ColorChoices::getBothSide()
{
    return bothSide;
}
