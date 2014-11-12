#include "Controller.hpp"
#include <boost/assert.hpp>
#include <SFGUI/Box.hpp>
#include <SFGUI/ScrolledWindow.hpp>
#include "components/ButtonBox.hpp"
#include <SFGUI/Notebook.hpp>
#include <boost/cast.hpp>

void Controller::enableWindow(const bool enable)
{
    if (enable) boardWindow->SetState(sfg::Widget::State::NORMAL);
    else boardWindow->SetState(sfg::Widget::State::INSENSITIVE);
}

void Controller::flagDown(Color loser)
{
    clocks.setFlagDown(loser);
    //messages.triggerEvent(EndGameMessage(!loser));
}

void Controller::moveMake(const std::vector<std::vector<Unit>>& position, int whiteTime, int blackTime)
{
    game.setTime(whiteTime, blackTime);
    game.startClock();

    //board.setupBoard(position);

    //messages.triggerEvent(MoveMessage(completeMove));
    //promotion choice if player move
}

void Controller::newGame(Color player, int time, const std::string &player_1, const std::string &player_2)
{
    messages.triggerEvent(NewGameMessage(player, time, player_1, player_2));
}

void Controller::settingsClicked()
{
    //enableWindow(false);
    //settingsWindow.setTree(resources.getTree());
    //settingsWindow.enable(true);
}

void Controller::settingsDone(const std::string& whitePrefix, const std::string& blackPrefix, const std::string& boardSuffix)
{
    settingsWindow.enable(false);
    enableWindow(true);
    //resources.reload(whitePrefix, blackPrefix, boardSuffix);
    //board.reload(game.getPosition());
}

void Controller::slotNewGame()
{
    sideChoice.enable(false);
    enableWindow(true);

    std::string toCheck = sfg::Context::Get().GetActiveWidget()->GetId();
    Color toSet;

    if      (toCheck == "whiteNewGame") toSet = Color::White;
    else if (toCheck == "blackNewGame") toSet = Color::Black;
    else {
        BOOST_ASSERT_MSG(toCheck == "bothNewGame", "Invalid widget requests new game");
        toSet = Color::Both;
    }

    messages.triggerEvent(NewGameMessage(toSet, 300, "White", "Black"));
}

void Controller::slotPromote()
{
    //    std::string toCheck = sfg::Context::Get().GetActiveWidget()->GetId();
    //    Piece whichPiece;

    //    if      (toCheck == "promoteQueen") whichPiece = Piece::Queen;
    //    else if (toCheck == "promoteBishop") whichPiece = Piece::Bishop;
    //    else if (toCheck == "promoteKnight") whichPiece = Piece::Knight;
    //    else{
    //        BOOST_ASSERT_MSG(toCheck == "promoteRook", "Invalid widget requests promotion");
    //        whichPiece = Piece::Rook;
    //    }



    //    promotionChoice = whichPiece;

    //    //update view
    //    //board.setPromotion(toPromote.square_2, whichPiece);

    //    //update model
    //    Color whichSide;
    //    if (game.turnColor()==Color::White) whichSide = Color::Black;
    //    else whichSide = Color::White;
    //    game.setPromotion(toPromote.square_2, {whichSide, whichPiece});

    //    client.makeMove(toPromote, whichPiece);

    //    promotionWindow->Show(false);
    //    enableWindow(true);
}

bool Controller::requestMove(const Move& move)
{
    if (!game.userTurn()) {
        //premove
        if (board.getColorOn(move.square_1) == game.getUserColor()) {
            premove = move;
            premoveOn = true;
            board.setArrow(move);
        }
        return false;
    }

    //    CompleteMove toCheck(game.getPosition(), move);
    //    if (toCheck.isLegal()){
    //        messages.triggerEvent(MoveMessage(toCheck));
    //        if (!toCheck.getNewBoard().wasPromotion) //pass to client only if it wasn't promotion
    client.makeMove(move);
    return true;
    //    }
    //    return false;
}

Controller::Controller(sf::Window &theWindow, sfg::Desktop &theDesktop):
    desktop(theDesktop),
    promotionWindow(sfg::Window::Create()),
    boardWindow(sfg::Window::Create(sfg::Window::BACKGROUND)),
    settingsButton(sfg::Button::Create("Settings")),
    board(theWindow),
    sideChoice(desktop),
    settingsWindow(desktop),
    premove({{0,0},{0,0}}), premoveOn(false),
    player1(sfg::Label::Create()),
    player2(sfg::Label::Create())
{
    boardWindow->SetRequisition(static_cast<sf::Vector2f>(theWindow.getSize()));

    board.getSignal().connect(boost::bind(&Controller::requestMove, this,_1));
    settingsWindow.settingsDone.connect(boost::bind(&Controller::settingsDone, this,_1,_2,_3));
    //client.positionReady.connect(boost::bind(&Controller::moveMake, this, _1, _2, _3));
    client.startGame.connect(boost::bind(&Controller::newGame, this, _1, _2, _3, _4));
    //client.gameEnd.connect(boost::bind(&Controller::setGameEnded, this, _1));
    client.textReady.connect(boost::bind(&NetWidgets::addLine, &netWindow, _1));
    netWindow.sendText.connect(boost::bind(&Client::toClient, &client, _1));

    //game.getWhiteTimer().connect(std::bind(&boardMaster::flagDown, this, bw::White));
    //game.getBlackTimer().connect(std::bind(&boardMaster::flagDown, this, bw::Black));

    sfg::Button::Ptr queenButton(sfg::Button::Create("Queen"));
    sfg::Button::Ptr bishopButton(sfg::Button::Create("Bishop"));
    sfg::Button::Ptr knightButton(sfg::Button::Create("Knight"));
    sfg::Button::Ptr rookButton(sfg::Button::Create("Rook"));

    queenButton->SetId("promoteQueen");
    bishopButton->SetId("promoteBishop");
    knightButton->SetId("promoteKnight");
    rookButton->SetId("promoteRook");

    sfg::Box::Ptr promotionBox(sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 3.f));

    promotionBox->PackEnd(queenButton);
    promotionBox->PackEnd(bishopButton);
    promotionBox->PackEnd(knightButton);
    promotionBox->PackEnd(rookButton);

    promotionWindow->Add(promotionBox);
    promotionWindow->SetPosition(sf::Vector2f(200.f,200.f));
    promotionWindow->SetTitle("Choose piece");

    queenButton->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&Controller::slotPromote,this));
    bishopButton->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&Controller::slotPromote,this));
    knightButton->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&Controller::slotPromote,this));
    rookButton->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&Controller::slotPromote,this));

    promotionWindow->Show(false);

    ButtonBox buttons;
    //buttons.resign()->GetSignal(sfg::Button::OnLeftClick).Connect(&boardMaster::resign, this);
    //buttons.draw()->GetSignal(sfg::Button::OnLeftClick).Connect(&boardMaster::offerDraw, this);
    buttons.newGame()->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&Controller::requestNewGame, this));
    buttons.flip()->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&Canvas::flipBoard, &board));
    buttons.settings()->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&Controller::settingsClicked, this));

    sfg::Table::Ptr mainLayout(sfg::Table::Create());
    mainLayout->SetRowSpacings(2.f);
    mainLayout->Attach(board.getBoardWidget(),{0, 0, 1, 12},sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 10.f, 0.f ));
    mainLayout->Attach(counters.getView(), {0, 10, 1, 2});
    mainLayout->Attach(player1, {1,0,1,1});
    mainLayout->Attach(clocks.getWhiteClock(),{1, 1, 1, 1});
    mainLayout->Attach(player2, {1,2,1,1});
    mainLayout->Attach(clocks.getBlackClock(),{1, 3, 1, 1});
    mainLayout->Attach(status.getView(),{1, 4, 1, 1});
    mainLayout->Attach(moveList.getView(),{1, 5, 1, 4});
    mainLayout->Attach(buttons.getWidget(),{0,12,2,2});

    //when making new game
    sideChoice.getWhiteSide()->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&Controller::slotNewGame, this));
    sideChoice.getBlackSide()->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&Controller::slotNewGame, this));
    sideChoice.getBothSide()->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&Controller::slotNewGame, this));

    desktop.Add(sideChoice.getWidget());
    desktop.Add(settingsWindow.getWidget());



    sfg::Notebook::Ptr notebook(sfg::Notebook::Create());
    notebook->AppendPage(mainLayout,sfg::Label::Create("Board"));
    notebook->AppendPage(netWindow.getWidget(),sfg::Label::Create("Server"));

    boardWindow->Add(notebook);


    desktop.Add(promotionWindow);
    desktop.Add(boardWindow);

    messages.connect("gameState", [this](const Message& message){
        const GameStateMessage* received = boost::polymorphic_downcast<const GameStateMessage*>(&message);
        game.setTime(received->white_time, received->black_time);
        game.startClock();
        //        if (received->move.getNewBoard().wasPromotion)
        //            handlePromotion(received->move.getMove());

        if (premoveOn)
        {
            premoveOn = false;
            board.clearArrows();
            requestMove(premove);
        }
    });

    messages.connect("newGame", [this](const Message& message){
        const NewGameMessage* received = boost::polymorphic_downcast<const NewGameMessage*>(&message);
        player1->SetText(received->p1);
        player2->SetText(received->p2);
        premoveOn = false;

        updateClocks();
    });

    messages.connect("resized", [this](const Message& message){
        const ResizeMessage* received = boost::polymorphic_downcast<const ResizeMessage*>(&message);
        boardWindow->SetAllocation({boardWindow->GetAbsolutePosition(),
                                    static_cast<sf::Vector2f>(received->window.getSize())});
    });

    messages.connect("endGame", [this](const Message& message){
        auto received = boost::polymorphic_downcast<const EndGameMessage*>(&message);
        premoveOn = false;
        board.clearArrows();
    });

    client.connect();

}

void Controller::update()
{
    client.update();
    board.display();
    //if (!game.ended())
    updateClocks();
}

void Controller::resign()
{
    //messages.triggerEvent(EndGameMessage(game.turnColor()));
}

void Controller::offerDraw()
{
    return; //TODO: forward this to the client
}


void Controller::requestNewGame()
{
    sideChoice.enable(true);
    enableWindow(false);
}

void Controller::updateClocks()
{
    game.update(); //update model
    clocks.update(game.getWhiteTime(), game.getBlackTime()); //update view
}
