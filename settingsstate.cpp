#include "settingsstate.h"
#include <SFGUI/Box.hpp>
#include <array>

settingsState::settingsState()://sfg::Desktop& theDesktop):
    //desktop(theDesktop)
    window(sfg::Window::Create()),
    closeButton(sfg::Button::Create("Close"))
{
    window->SetRequisition(sf::Vector2f(100.f,100.f));
    window->SetTitle("Settings");
    window->Show(false);

    std::array<std::string,12> colors = {"Black","Blue","Blue2","Brown","Green","Green2","Red","Red2",
                                         "Violet","Violet2","White","Yellow"};

    sfg::Box::Ptr buttonLayout = sfg::Box::Create(sfg::Box::VERTICAL);
    sfg::RadioButtonGroup::Ptr buttonGroup(sfg::RadioButtonGroup::Create());

    for (auto& color : colors)
    {
        sfg::RadioButton::Ptr button(sfg::RadioButton::Create(color,buttonGroup));
        if (color=="Black") button->SetActive(true);
        buttonLayout->Pack(button);
    }

    window->Add(buttonLayout);
    window->Add(closeButton);
}

void settingsState::makeVisible()
{
    window->Show(true);
}
