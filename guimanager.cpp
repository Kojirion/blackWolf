#include "guimanager.h"

void guiManager::settingsClicked()
{
    stateManager.process_event(settingsOpen());
}

void guiManager::settingsClosed()
{
    stateManager.process_event(settingsClose());
}

guiManager::guiManager():
    window(sf::VideoMode(600, 600), "Black Wolf"),
    stateManager(boost::ref(desktop))
{
    window.setFramerateLimit(60);
    stateManager.start();

    //icon.loadFromFile("Graphics/Boardbrown.jpg");
    //window.setIcon(32,32,icon.getPixelsPtr());
}

void guiManager::run()
{
    //connect settings close to button to signal
    //guiManager's involment could be done away with
    stateManager.settingsData.closeButton->
            GetSignal(sfg::Button::OnLeftClick).Connect(&guiManager::settingsClosed, this);

    boardMaster boss(window, desktop);

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

    sfg::Button::Ptr settingsButton(sfg::Button::Create("Settings"));
    settingsButton->GetSignal(sfg::Button::OnLeftClick).Connect(&guiManager::settingsClicked, this);


    sfg::Table::Ptr buttonLayout(sfg::Table::Create());
    buttonLayout->SetRowSpacings(3.f);
    buttonLayout->SetColumnSpacings(3.f);

    buttonLayout->Attach(resignButton,{0,0,1,1});
    buttonLayout->Attach(drawButton,{0,1,1,1});
    buttonLayout->Attach(newGameButton,{1,0,1,1});
    buttonLayout->Attach(flipButton,{1,1,1,1});
    buttonLayout->Attach(settingsButton,{2,0,1,1});


    sfg::Table::Ptr mainLayout(sfg::Table::Create());
    mainLayout->SetRowSpacings(5.f);
    mainLayout->Attach(boss.window_,{0, 0, 1, 8},sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 10.f, 0.f ));
    mainLayout->Attach(boss.whiteClockCanvas_,{1, 0, 1, 1});
    mainLayout->Attach(boss.blackClockCanvas_,{1, 1, 1, 1});
    mainLayout->Attach(boss.turnLabel_,{1, 2, 1, 1});
    mainLayout->Attach(moveListWindow,{1, 3, 1, 4});
    mainLayout->Attach(buttonLayout,{0,8,2,2});

    //when making new game
    boss.sideChoiceWindow->SetPosition(sf::Vector2f(200.f,200.f));
    boss.sideChoiceWindow->SetRequisition(sf::Vector2f(200.f,200.f));
    boss.sideChoiceWindow->SetTitle("Choose color");

    sfg::Box::Ptr sideChoiceBox(sfg::Box::Create(sfg::Box::VERTICAL, 5.f));
    sfg::Button::Ptr whiteSide(sfg::Button::Create("White"));
    sfg::Button::Ptr blackSide(sfg::Button::Create("Black"));
    sfg::Button::Ptr bothSide(sfg::Button::Create("Both"));

    whiteSide->GetSignal(sfg::Button::OnLeftClick).Connect(&boardMaster::whiteNewGame, &boss);
    blackSide->GetSignal(sfg::Button::OnLeftClick).Connect(&boardMaster::blackNewGame, &boss);
    bothSide->GetSignal(sfg::Button::OnLeftClick).Connect(&boardMaster::bothNewGame, &boss);

    sideChoiceBox->Pack(whiteSide);
    sideChoiceBox->Pack(blackSide);
    sideChoiceBox->Pack(bothSide);
    boss.sideChoiceWindow->Add(sideChoiceBox);
    boss.sideChoiceWindow->Show(false);
    boss.desktop.Add(boss.sideChoiceWindow);





    //boardWindow->SetTitle( "Board" );
    boss.boardWindow->Add(mainLayout);


    boss.desktop.Add(boss.choiceWindow);
    boss.desktop.Add(boss.boardWindow);


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


