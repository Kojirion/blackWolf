#include "Controller.hpp"
#include <boost/assert.hpp>
#include <SFGUI/Box.hpp>
#include <SFGUI/ScrolledWindow.hpp>
#include "components/ButtonBox.hpp"
#include <SFGUI/Notebook.hpp>
#include <boost/cast.hpp>

void Controller::setGameEnded(Color result)
{
    //update model
    game.setResult(result);

    //update view
    //status.setResult(result);
    board.setResult(result);
}

void Controller::enableWindow(const bool enable)
{
    if (enable) boardWindow->SetState(sfg::Widget::NORMAL);
    else boardWindow->SetState(sfg::Widget::INSENSITIVE);
}

void Controller::flagDown(Color loser)
{
    clocks.setFlagDown(loser);
    setGameEnded(!loser);
}

void Controller::handlePromotion(const Move& move)
{
    toPromote = move;

    if (!game.userTurn()){ //techinically, it still is
        //enable promotion window
        promotionWindow->Show(true);
        desktop.BringToFront(promotionWindow);

        //disable board window
        enableWindow(false);
    }else{
        //promotionChoiceMade(chessAi.getPromotionChoice());
    }
}

void Controller::settingsClicked()
{
    enableWindow(false);
    settingsWindow.setTree(resources.getTree());
    settingsWindow.enable(true);
}

void Controller::settingsDone(std::string whitePrefix, std::string blackPrefix, std::string boardSuffix)
{
    settingsWindow.enable(false);
    enableWindow(true);
    resources.reload(whitePrefix, blackPrefix, boardSuffix);
    board.reload(game.getPosition());
}

void Controller::moveMake(const CompleteMove &move)
{
    messages.triggerEvent(MoveMessage("moveMade", move));

    if (premoveOn)
    {
        premoveOn = false;
        board.clearArrows();
        requestMove(premove);
    }

    //check for game end?
}

void Controller::newGame(Color whoUser, int time, std::string p1, std::string p2)
{
    player1->SetText(p1);
    player2->SetText(p2);

    game.newGame(whoUser, time);

    board.resetFor(whoUser);

    board.setPosition(game.getPosition());

    moveList.reset();

    //status.setToPlay(Color::White);

    updateClocks();

}

void Controller::aiTurn()
{
    Move moveToMake = chessAi.getMove();

    CompleteMove toCheck(game.getPosition(), moveToMake);
    BOOST_ASSERT_MSG(toCheck.isLegal(), "Engine tries to play illegal move");

    moveMake(toCheck);
}

void Controller::promotionChoiceMade(Piece whichPiece)
{
    promotionChoice = whichPiece;

    //update view
    board.setPromotion(toPromote.square_2, whichPiece);

    //update model
    Color whichSide;
    if (game.turnColor()==Color::White) whichSide = Color::Black;
    else whichSide = Color::White;
    game.setPromotion(toPromote.square_2, {whichSide, whichPiece});

    promotionWindow->Show(false);
    enableWindow(true);

//    if (!game.userTurn()) //means user made promotion, so send to client
//        fics.makeMove(toPromoteRow1, toPromoteCol1, toPromoteRow2, toPromoteCol2, whichPiece);
}

void Controller::promoteQueen()
{
    promotionChoiceMade(Piece::Queen);
}

void Controller::promoteBishop()
{
    promotionChoiceMade(Piece::Bishop);
}

void Controller::promoteKnight()
{
    promotionChoiceMade(Piece::Knight);
}

void Controller::promoteRook()
{
    promotionChoiceMade(Piece::Rook);
}

void Controller::whiteNewGame()
{
    sideChoice.enable(false);
    enableWindow(true);
    newGame(Color::White, 300, "White", "Black");
}

void Controller::blackNewGame()
{
    sideChoice.enable(false);
    enableWindow(true);
    newGame(Color::Black, 300, "White", "Black");
}

void Controller::bothNewGame()
{
    sideChoice.enable(false);
    enableWindow(true);
    newGame(Color::Both, 300, "White", "Black");
}

bool Controller::requestMove(const Move& move)
{
    if (!game.userTurn()) //set premove
    {
        premove = move;
        board.setArrow(move);
        return false;
    }

    CompleteMove toCheck(game.getPosition(), move);
    if (toCheck.isLegal()){
        moveMake(toCheck);
//        if (!toCheck.getNewBoard().wasPromotion) //pass to client only if it wasn't promotion
//            fics.makeMove(row1, col1, row2, col2);
        return true;
    }
    return false;
}

Controller::Controller(sf::Window &theWindow, sfg::Desktop &theDesktop):
    desktop(theDesktop),
    promotionWindow(sfg::Window::Create()),
    boardWindow(sfg::Window::Create(sfg::Window::BACKGROUND)),
    settingsButton(sfg::Button::Create("Settings")),    
    board(theWindow,resources),    
    sideChoice(desktop),    
    settingsWindow(desktop),
    premove({{0,0},{0,0}}), premoveOn(false),
    player1(sfg::Label::Create()),
    player2(sfg::Label::Create()),
    toPromote({{0,0},{0,0}}), promotionChoice(Piece::None)
{
    board.getSignal().connect(boost::bind(&Controller::requestMove, this,_1));
    settingsWindow.settingsDone.connect(boost::bind(&Controller::settingsDone, this,_1,_2,_3));
    fics.startGame.connect(boost::bind(&Controller::newGame, this, _1, _2, _3, _4));
    fics.gameEnd.connect(boost::bind(&Controller::setGameEnded, this, _1));
    fics.textReady.connect(boost::bind(&NetWidgets::addLine, &netWindow, _1));
    netWindow.sendText.connect(boost::bind(&Client::toClient, &fics, _1));

    //game.getWhiteTimer().connect(std::bind(&boardMaster::flagDown, this, bw::White));
    //game.getBlackTimer().connect(std::bind(&boardMaster::flagDown, this, bw::Black));

    sfg::Button::Ptr queenButton(sfg::Button::Create("Queen"));
    sfg::Button::Ptr bishopButton(sfg::Button::Create("Bishop"));
    sfg::Button::Ptr knightButton(sfg::Button::Create("Knight"));
    sfg::Button::Ptr rookButton(sfg::Button::Create("Rook"));

    sfg::Box::Ptr promotionBox(sfg::Box::Create(sfg::Box::HORIZONTAL, 3.f));

    promotionBox->PackEnd(queenButton);
    promotionBox->PackEnd(bishopButton);
    promotionBox->PackEnd(knightButton);
    promotionBox->PackEnd(rookButton);

    promotionWindow->Add(promotionBox);
    promotionWindow->SetPosition(sf::Vector2f(200.f,200.f));
    promotionWindow->SetTitle("Choose piece");

    queenButton->GetSignal(sfg::Widget::OnLeftClick).Connect(&Controller::promoteQueen,this);
    bishopButton->GetSignal(sfg::Widget::OnLeftClick).Connect(&Controller::promoteBishop,this);
    knightButton->GetSignal(sfg::Widget::OnLeftClick).Connect(&Controller::promoteKnight,this);
    rookButton->GetSignal(sfg::Widget::OnLeftClick).Connect(&Controller::promoteRook,this);

    promotionWindow->Show(false);

    ButtonBox buttons;
    //buttons.resign()->GetSignal(sfg::Button::OnLeftClick).Connect(&boardMaster::resign, this);
    //buttons.draw()->GetSignal(sfg::Button::OnLeftClick).Connect(&boardMaster::offerDraw, this);
    //buttons.newGame()->GetSignal(sfg::Button::OnLeftClick).Connect(&boardMaster::requestNewGame, this);
    buttons.flip()->GetSignal(sfg::Button::OnLeftClick).Connect(&Canvas::flipBoard, &board);
    buttons.settings()->GetSignal(sfg::Button::OnLeftClick).Connect(&Controller::settingsClicked, this);

    sfg::Table::Ptr mainLayout(sfg::Table::Create());
    mainLayout->SetRowSpacings(2.f);
    mainLayout->Attach(board.getBoardWidget(),{0, 0, 1, 12},sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 10.f, 0.f ));
    mainLayout->Attach(player1, {1,0,1,1});
    mainLayout->Attach(clocks.getWhiteClock(),{1, 1, 1, 1});
    mainLayout->Attach(player2, {1,2,1,1});
    mainLayout->Attach(clocks.getBlackClock(),{1, 3, 1, 1});
    mainLayout->Attach(status.getView(),{1, 4, 1, 1});
    mainLayout->Attach(moveList.getView(),{1, 5, 1, 4});
    mainLayout->Attach(buttons.getWidget(),{0,12,2,2});

    //when making new game
    sideChoice.getWhiteSide()->GetSignal(sfg::Button::OnLeftClick).Connect(&Controller::whiteNewGame, this);
    sideChoice.getBlackSide()->GetSignal(sfg::Button::OnLeftClick).Connect(&Controller::blackNewGame, this);
    sideChoice.getBothSide()->GetSignal(sfg::Button::OnLeftClick).Connect(&Controller::bothNewGame, this);

    desktop.Add(sideChoice.getWidget());
    desktop.Add(settingsWindow.getWidget());



    sfg::Notebook::Ptr notebook(sfg::Notebook::Create());
    notebook->AppendPage(mainLayout,sfg::Label::Create("Board"));
    notebook->AppendPage(netWindow.getWidget(),sfg::Label::Create("Server"));

    boardWindow->Add(notebook);


    desktop.Add(promotionWindow);
    desktop.Add(boardWindow);

    //fics.connect();
    newGame(Color::Both, 300, "lol", "what");

    //    if (!chessAi.load()) newGame(bw::White | bw::Black);
    //    else newGame(bw::White);

    messages.connect("moveMade", [this](const Message& message){
        const MoveMessage* received = boost::polymorphic_downcast<const MoveMessage*>(&message);
        if (received->move.getNewBoard().wasPromotion)
            handlePromotion(received->move.getMove());
    });





}

void Controller::update()
{
    //fics.update();
    board.display();    
    if (!game.ended()) updateClocks();
}

void Controller::resign()
{
    setGameEnded(!game.getUserColor()); //should be turncolor
}

void Controller::offerDraw()
{
    return; //ai rejects all offers for now
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
