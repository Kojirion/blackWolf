#include "BoardMaster.hpp"
#include <boost/assert.hpp>
#include <SFGUI/Box.hpp>
#include <SFGUI/ScrolledWindow.hpp>
#include "components/ButtonBox.hpp"
#include <SFGUI/Notebook.hpp>
#include <boost/cast.hpp>

void BoardMaster::setGameEnded(Color result)
{
    //update model
    game.setResult(result);

    //update view
    status.setResult(result);
    board.setResult(result);
}

void BoardMaster::enableWindow(const bool enable)
{
    if (enable) boardWindow->SetState(sfg::Widget::NORMAL);
    else boardWindow->SetState(sfg::Widget::INSENSITIVE);
}

void BoardMaster::flagDown(Color loser)
{
    clocks.setFlagDown(loser);
    setGameEnded(!loser);
}

void BoardMaster::handlePromotion(const Move& move)
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

void BoardMaster::settingsClicked()
{
    enableWindow(false);
    settingsWindow.setTree(resources.getTree());
    settingsWindow.enable(true);
}

void BoardMaster::settingsDone(std::string whitePrefix, std::string blackPrefix, std::string boardSuffix)
{
    settingsWindow.enable(false);
    enableWindow(true);
    resources.reload(whitePrefix, blackPrefix, boardSuffix);
    board.reload(game.getPosition());
}

void BoardMaster::moveMake(const CompleteMove &move)
{
    messageSystem.triggerEvent(MoveMessage("moveMade", move));
//    const Move& toMake = move.getMove();

//    board.moveMake(move); //update view
//    board.releasePiece(); //release piece
//    game.setPosition(move.getNewBoard()); //update model

//    //handle promotion AND update the engine, depending on whether it was or not
//    if (game.getPosition().wasPromotion){
//        handlePromotion(toMake);
//        //if (!game.userBoth()) chessAi.makeMove(originRow,originCol,destRow,destCol, promotionChoice);
//    }else{
//        //if (!game.userBoth()) chessAi.makeMove(originRow,originCol,destRow,destCol);
//    }

//    //update move counter and move list widget
//    moveList.addMove(toMake,game.getPlyCount());

//    //check for game end or switch turn
//    //if (move.isCheckmate()) setGameEnded(-game.turnColor());
//    //if (move.isStalemate()) setGameEnded(bw::White | bw::Black);
//    if (!game.ended()) switchTurn();
}

void BoardMaster::networkMoveMake(const Move& move, int whiteTime, int blackTime, Piece promotionChoice)
{
//    game.setTime(whiteTime, blackTime);
//    game.startClock(); //this just means an unnecessary stop

//    //now an ugly way to say: if we already made the move on the board,
//    //we don't care what the client sent
//    if (game.getPosition()(move.square_1).piece==Piece::None) return;

//    CompleteMove c_move(game.getPosition(), move);

//    board.moveMake(c_move); //update view
//    game.setPosition(c_move.getNewBoard()); //update model

//    if (promotionChoice != Piece::None){
//        toPromote = move;
//        promotionChoiceMade(promotionChoice);
//    }

    //handle promotion AND update the engine, depending on whether it was or not
//    if (game.getPosition().wasPromotion){
//        handlePromotion(destRow, destCol);
//        if (!game.userBoth()) chessAi.makeMove(originRow,originCol,destRow,destCol, promotionChoice);
//    }else{
//        if (!game.userBoth()) chessAi.makeMove(originRow,originCol,destRow,destCol);
//    }

    //update move counter and move list widget
    //moveList.addMove(move,game.getPlyCount());

    //check for game end or switch turn
    //if (move.isCheckmate()) setGameEnded(-game.turnColor());
    //if (move.isStalemate()) setGameEnded(bw::White | bw::Black);
    //if (!game.ended()) switchTurn();

}

void BoardMaster::newGame(Color whoUser, int time, std::string p1, std::string p2)
{
    player1->SetText(p1);
    player2->SetText(p2);

    game.newGame(whoUser, time);

    board.resetFor(whoUser);

    board.setPosition(game.getPosition());

    moveList.reset();

    status.setToPlay(Color::White);

    updateClocks();

}

void BoardMaster::aiTurn()
{
    Move moveToMake = chessAi.getMove();

    CompleteMove toCheck(game.getPosition(), moveToMake);
    BOOST_ASSERT_MSG(toCheck.isLegal(), "Engine tries to play illegal move");

    moveMake(toCheck);
}

void BoardMaster::promotionChoiceMade(Piece whichPiece)
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

void BoardMaster::promoteQueen()
{
    promotionChoiceMade(Piece::Queen);
}

void BoardMaster::promoteBishop()
{
    promotionChoiceMade(Piece::Bishop);
}

void BoardMaster::promoteKnight()
{
    promotionChoiceMade(Piece::Knight);
}

void BoardMaster::promoteRook()
{
    promotionChoiceMade(Piece::Rook);
}

void BoardMaster::whiteNewGame()
{
    sideChoice.enable(false);
    enableWindow(true);
    newGame(Color::White, 300, "White", "Black");
}

void BoardMaster::blackNewGame()
{
    sideChoice.enable(false);
    enableWindow(true);
    newGame(Color::Black, 300, "White", "Black");
}

void BoardMaster::bothNewGame()
{
    sideChoice.enable(false);
    enableWindow(true);
    newGame(Color::Both, 300, "White", "Black");
}

bool BoardMaster::requestMove(const Move& move)
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

BoardMaster::BoardMaster(sf::Window &theWindow, sfg::Desktop &theDesktop):
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
    board.getSignal().connect(boost::bind(&BoardMaster::requestMove, this,_1));
    settingsWindow.settingsDone.connect(boost::bind(&BoardMaster::settingsDone, this,_1,_2,_3));
    fics.positionReady.connect(boost::bind(&BoardMaster::networkMoveMake, this, _1, _2, _3, _4));
    fics.startGame.connect(boost::bind(&BoardMaster::newGame, this, _1, _2, _3, _4));
    fics.gameEnd.connect(boost::bind(&BoardMaster::setGameEnded, this, _1));
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

    queenButton->GetSignal(sfg::Widget::OnLeftClick).Connect(&BoardMaster::promoteQueen,this);
    bishopButton->GetSignal(sfg::Widget::OnLeftClick).Connect(&BoardMaster::promoteBishop,this);
    knightButton->GetSignal(sfg::Widget::OnLeftClick).Connect(&BoardMaster::promoteKnight,this);
    rookButton->GetSignal(sfg::Widget::OnLeftClick).Connect(&BoardMaster::promoteRook,this);

    promotionWindow->Show(false);

    ButtonBox buttons;
    //buttons.resign()->GetSignal(sfg::Button::OnLeftClick).Connect(&boardMaster::resign, this);
    //buttons.draw()->GetSignal(sfg::Button::OnLeftClick).Connect(&boardMaster::offerDraw, this);
    //buttons.newGame()->GetSignal(sfg::Button::OnLeftClick).Connect(&boardMaster::requestNewGame, this);
    buttons.flip()->GetSignal(sfg::Button::OnLeftClick).Connect(&Canvas::flipBoard, &board);
    buttons.settings()->GetSignal(sfg::Button::OnLeftClick).Connect(&BoardMaster::settingsClicked, this);

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
    sideChoice.getWhiteSide()->GetSignal(sfg::Button::OnLeftClick).Connect(&BoardMaster::whiteNewGame, this);
    sideChoice.getBlackSide()->GetSignal(sfg::Button::OnLeftClick).Connect(&BoardMaster::blackNewGame, this);
    sideChoice.getBothSide()->GetSignal(sfg::Button::OnLeftClick).Connect(&BoardMaster::bothNewGame, this);

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

    messageSystem.connect("moveMade", [this](const Message& message){
        const MoveMessage* received = boost::polymorphic_downcast<const MoveMessage*>(&message);
        if (received->move.getNewBoard().wasPromotion)
            handlePromotion(received->move.getMove());
    });





}

void BoardMaster::display() //should rename to update
{
    //fics.update();
    board.display();    
    if (!game.ended()) updateClocks();
}

void BoardMaster::switchTurn()
{
    status.setToPlay(game.turnColor());
    game.switchTurn();

    if (premoveOn)
    {
        premoveOn = false;
        board.clearArrows();
        requestMove(premove);
    }

//    if (!game.userTurn()){
//        aiTurn();
//    }
}


void BoardMaster::resign()
{
    setGameEnded(!game.getUserColor()); //should be turncolor
}

void BoardMaster::offerDraw()
{
    return; //ai rejects all offers for now
}


void BoardMaster::requestNewGame()
{
    sideChoice.enable(true);
    enableWindow(false);
}

void BoardMaster::updateClocks()
{
    game.update(); //update model
    clocks.update(game.getWhiteTime(), game.getBlackTime()); //update view
}
