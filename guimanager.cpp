#include "guimanager.h"

guiManager::guiManager():
    window(sf::VideoMode(600, 600), "Black Wolf")
{
}

void guiManager::run()
{
    sfg::Window::Ptr boardWindow(sfg::Window::Create());

    boardMaster boss(window, boardWindow);

    sfg::ScrolledWindow::Ptr moveListWindow(sfg::ScrolledWindow::Create());
    moveListWindow->SetRequisition(sf::Vector2f(110.f,0.f));
    moveListWindow->SetScrollbarPolicy( sfg::ScrolledWindow::HORIZONTAL_NEVER | sfg::ScrolledWindow::VERTICAL_AUTOMATIC );
    moveListWindow->AddWithViewport(boss.moveList);

    sfg::Button::Ptr resignButton(sfg::Button::Create("Resign"));
    resignButton->GetSignal(sfg::Button::OnLeftClick).Connect(&boardMaster::resign, &boss);

    sfg::Button::Ptr drawButton(sfg::Button::Create("Offer draw"));
    drawButton->GetSignal(sfg::Button::OnLeftClick).Connect(&boardMaster::offerDraw, &boss);

    sfg::Button::Ptr newGameButton(sfg::Button::Create("New game"));
    newGameButton->GetSignal(sfg::Button::OnLeftClick).Connect(&boardMaster::requestNewGame, &boss);

    sfg::Button::Ptr flipButton(sfg::Button::Create("Flip board"));
    flipButton->GetSignal(sfg::Button::OnLeftClick).Connect(&boardMaster::flipBoard, &boss);


    sfg::Table::Ptr buttonLayout(sfg::Table::Create());
    buttonLayout->SetRowSpacings(3.f);
    buttonLayout->SetColumnSpacings(3.f);

    buttonLayout->Attach(resignButton,{0,0,1,1});
    buttonLayout->Attach(drawButton,{0,1,1,1});
    buttonLayout->Attach(newGameButton,{1,0,1,1});
    buttonLayout->Attach(flipButton,{1,1,1,1});


    sfg::Table::Ptr mainLayout(sfg::Table::Create());
    mainLayout->SetRowSpacings(5.f);
    mainLayout->Attach(boss.window_,{0, 0, 1, 8},sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 10.f, 0.f ));
    mainLayout->Attach(boss.whiteClockCanvas_,{1, 0, 1, 1});
    mainLayout->Attach(boss.blackClockCanvas_,{1, 1, 1, 1});
    mainLayout->Attach(boss.turnLabel_,{1, 2, 1, 1});
    mainLayout->Attach(moveListWindow,{1, 3, 1, 4});
    mainLayout->Attach(buttonLayout,{0,8,2,2});





    //boardWindow->SetTitle( "Board" );
    boardWindow->Add(mainLayout);


    boss.desktop.Add(boss.choiceWindow);
    boss.desktop.Add(boardWindow);


    window.resetGLStates();
    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            boss.desktop.HandleEvent(event);

            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed){
                //if event.mo
            }
        }

        boss.desktop.Update(clock.restart().asSeconds());

        window.clear();
        boss.display();
        sfgui_.Display(window);
        window.display();
    }

}
