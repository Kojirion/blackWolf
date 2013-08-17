#include "settingsstate.h"

settingsState::settingsState()://sfg::Desktop& theDesktop):
    //desktop(theDesktop)
    window(sfg::Window::Create())
{
    window->SetRequisition(sf::Vector2f(100.f,100.f));
    window->SetTitle("Settings");
    window->Show(false);
}

void settingsState::makeVisible()
{
    window->Show(true);
}
