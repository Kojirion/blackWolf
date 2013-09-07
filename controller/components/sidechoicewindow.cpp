#include "sidechoicewindow.h"
#include <SFGUI/Box.hpp>

sideChoiceWindow::sideChoiceWindow(sfg::Desktop &theDesktop):    
    window(sfg::Window::Create()),
    desktop(theDesktop),
    whiteSide(sfg::Button::Create("White")),
    blackSide(sfg::Button::Create("Black")),
    bothSide(sfg::Button::Create("Both"))
{
    window->SetPosition(sf::Vector2f(200.f,200.f));
    window->SetRequisition(sf::Vector2f(200.f,200.f));
    window->SetTitle("Choose color");

    sfg::Box::Ptr sideChoiceBox(sfg::Box::Create(sfg::Box::VERTICAL, 5.f));

    sideChoiceBox->Pack(whiteSide);
    sideChoiceBox->Pack(blackSide);
    sideChoiceBox->Pack(bothSide);
    window->Add(sideChoiceBox);

    enable(false);
}

void sideChoiceWindow::enable(bool doEnable)
{
    if (doEnable) desktop.BringToFront(window);
    window->Show(doEnable);
}

sfg::Widget::Ptr sideChoiceWindow::getWidget()
{
    return window;
}

sfg::Button::Ptr sideChoiceWindow::getWhiteSide()
{
    return whiteSide;
}

sfg::Button::Ptr sideChoiceWindow::getBlackSide()
{
    return blackSide;
}

sfg::Button::Ptr sideChoiceWindow::getBothSide()
{
    return bothSide;
}
