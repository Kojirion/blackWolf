#include "Settings.hpp"
#include <SFGUI/Box.hpp>
#include <SFGUI/Table.hpp>
#include <SFGUI/Frame.hpp>
#include <SFGUI/RadioButton.hpp>
#include <SFGUI/RadioButtonGroup.hpp>

void Settings::requestClose()
{
    int iMax(colors.size());
    for (int i=0; i<iMax; ++i)
    {
        if (whiteButtons[i]->IsActive())
        {
            toSetWhite = colors[i];            
        }

        if (blackButtons[i]->IsActive())
        {
            toSetBlack = colors[i];
        }
    }

    iMax = boardColors.size();
    for (int i=0; i<iMax; ++i)
    {
        if (boardButtons[i]->IsActive())
        {
            toSetBoard = boardColors[i];
            break;
        }
    }

    //if (toSetWhite == toSetBlack)

    settingsDone(toSetWhite,toSetBlack,toSetBoard);
}

//namespace sfg {typedef RadioButton::RadioButtonGroup RadioButtonGroup; }

Settings::Settings(sfg::Desktop &theDesktop):
    window(sfg::Window::Create()),
    desktop(theDesktop),
    closeButton(sfg::Button::Create("Close")),
    colors{{"Black","Blue","Blue2","Brown","Green","Green2","Red","Red2",
                                                          "Violet","Violet2","White","Yellow"}},
    boardColors{{"Black","Blue","Brown"}}
{
    window->SetRequisition(sf::Vector2f(100.f,100.f));
    window->SetTitle("Settings");
    window->Show(false);



    sfg::Box::Ptr buttonLayout1 = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
    sfg::RadioButtonGroup::Ptr buttonGroup1(sfg::RadioButtonGroup::Create());

    sfg::Box::Ptr buttonLayout2 = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
    sfg::RadioButtonGroup::Ptr buttonGroup2(sfg::RadioButtonGroup::Create());

    int iMax(colors.size());
    for (int i=0; i<iMax; ++i)
    {
        whiteButtons[i] = sfg::RadioButton::Create(colors[i],buttonGroup1);
        blackButtons[i] = sfg::RadioButton::Create(colors[i],buttonGroup2);

        buttonLayout1->Pack(whiteButtons[i]);
        buttonLayout2->Pack(blackButtons[i]);
    }

    sfg::Frame::Ptr frame1(sfg::Frame::Create("White"));
    frame1->Add(buttonLayout1);

    sfg::Frame::Ptr frame2(sfg::Frame::Create("Black"));
    frame2->Add(buttonLayout2);

    sfg::Box::Ptr buttonLayout3 = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
    sfg::RadioButtonGroup::Ptr buttonGroup3(sfg::RadioButtonGroup::Create());

    iMax = boardColors.size();
    for (int i=0; i<iMax; ++i)
    {
        boardButtons[i] = sfg::RadioButton::Create(boardColors[i],buttonGroup3);

        buttonLayout3->Pack(boardButtons[i]);
    }

    sfg::Frame::Ptr frame3(sfg::Frame::Create("Board"));
    frame3->Add(buttonLayout3);

    sfg::Table::Ptr mainLayout(sfg::Table::Create());
    mainLayout->SetRowSpacings(3.f);
    mainLayout->SetColumnSpacings(3.f);

    mainLayout->Attach(frame1,{0,0,1,1});
    mainLayout->Attach(frame2,{1,0,1,1});
    mainLayout->Attach(frame3,{2,0,1,1});
    mainLayout->Attach(closeButton,{2,1,1,1});

    closeButton->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&Settings::requestClose,this));

    window->Add(mainLayout);
}

void Settings::enable(bool doEnable)
{
    if (doEnable) desktop.BringToFront(window);
    window->Show(doEnable);
}

sfg::Widget::Ptr Settings::getWidget()
{
    return window;
}

void Settings::setTree(const boost::property_tree::ptree pt)
{
    toSetWhite = pt.get<std::string>("whitePrefix");
    toSetBlack = pt.get<std::string>("blackPrefix");
    toSetBoard = pt.get<std::string>("boardSuffix");

    int iMax = colors.size();
    for (int i=0; i<iMax; ++i)
    {
        if (colors[i] == toSetWhite) whiteButtons[i]->SetActive(true);
        if (colors[i] == toSetBlack) blackButtons[i]->SetActive(true);
    }

    iMax = boardColors.size();
    for (int i=0; i<iMax; ++i)
    {
       if (boardColors[i] == toSetBoard) boardButtons[i]->SetActive(true);
    }


}

