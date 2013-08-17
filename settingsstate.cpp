#include "settingsstate.h"

settingsState::settingsState()://sfg::Desktop& theDesktop):
    //desktop(theDesktop)
    window(sfg::Window::Create()),
    closeButton(sfg::Button::Create("Close"))
{
    window->SetRequisition(sf::Vector2f(100.f,100.f));
    window->SetTitle("Settings");
    window->Show(false);


    window->Add(closeButton);
}

void settingsState::makeVisible()
{
    window->Show(true);
}
