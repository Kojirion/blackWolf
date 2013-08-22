#include "settingsstate.h"
#include <SFGUI/Box.hpp>
#include <SFGUI/Table.hpp>

void settingsState::requestClose()
{
    std::string toSetWhite;
    for (int i=0; i<colors.size(); ++i)
    {
        if (whiteButtons[i]->IsActive())
        {
            toSetWhite = colors[i];
            break;
        }
    }

    settingsDone(toSetWhite,"Black","brown");
}

settingsState::settingsState(sfg::Desktop &theDesktop):
    desktop(theDesktop),
    window(sfg::Window::Create()),
    closeButton(sfg::Button::Create("Close")),
    colors{{"Black","Blue","Blue2","Brown","Green","Green2","Red","Red2",
                                                          "Violet","Violet2","White","Yellow"}}
{
    window->SetRequisition(sf::Vector2f(100.f,100.f));
    window->SetTitle("Settings");
    window->Show(false);

    sfg::Box::Ptr buttonLayout = sfg::Box::Create(sfg::Box::VERTICAL);
    sfg::RadioButtonGroup::Ptr buttonGroup(sfg::RadioButtonGroup::Create());

    for (int i=0; i<colors.size(); ++i)
    {
        whiteButtons[i] = (sfg::RadioButton::Create(colors[i],buttonGroup));
        if (colors[i]=="Black") whiteButtons[i]->SetActive(true);
        buttonLayout->Pack(whiteButtons[i]);
    }

    sfg::Table::Ptr mainLayout(sfg::Table::Create());
    mainLayout->Attach(buttonLayout,{0,0,1,1});
    mainLayout->Attach(closeButton,{1,1,1,1});

    closeButton->GetSignal(sfg::Widget::OnLeftClick).Connect(&settingsState::requestClose,this);

    window->Add(mainLayout);
}

void settingsState::enable(bool doEnable)
{
    if (doEnable) desktop.BringToFront(window);
    window->Show(doEnable);
}

sfg::Widget::Ptr settingsState::getWidget()
{
    return window;
}

