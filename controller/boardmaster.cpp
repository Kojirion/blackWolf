#include "boardmaster.h"
#include <boost/assert.hpp>
#include <SFGUI/Box.hpp>
#include <SFGUI/ScrolledWindow.hpp>
#include "components/buttonbox.h"
#include <SFGUI/Notebook.hpp>

void boardMaster::setGameEnded(Color result)
{
    //update model
    game.setResult(result);

    //update view
    status.setResult(result);
    board.setResult(result);
}

void boardMaster::enableWindow(const bool enable)
{
    if (enable) boardWindow->SetState(sfg::Widget::NORMAL);
    else boardWindow->SetState(sfg::Widget::INSENSITIVE);
}

void boardMaster::flagDown(Color loser)
{
    clocks.setFlagDown(loser);
    setGameEnded(!loser);
}

void boardMaster::handlePromotion(const Move& move)
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

void boardMaster::settingsClicked()
{
    enableWindow(false);
    settingsWindow.setTree(resources.getTree());
    settingsWindow.enable(true);
}

void boardMaster::settingsDone(std::string whitePrefix, std::string blackPrefix, std::string boardSuffix)
{
    settingsWindow.enable(false);
    enableWindow(true);
    resources.reload(whitePrefix, blackPrefix, boardSuffix);
    board.reload(game.getPosition());
}

void boardMaster::moveMake(const completeMove &move)
{
    const int originRow = move.getRow1();
    const int originCol = move.getCol1();
    const int destRow = move.getRow2();
    const int destCol = move.getCol2();

    board.moveMake(move); //update view
    board.releasePiece(); //release piece
    game.setPosition(move.getNewBoard()); //update model

    //handle promotion AND update the engine, depending on whether it was or not
    if (game.getPosition().wasPromotion){
        handlePromotion({{originRow, originCol}, {destRow, destCol}});
        //if (!game.userBoth()) chessAi.makeMove(originRow,originCol,destRow,destCol, promotionChoice);
    }else{
        //if (!game.userBoth()) chessAi.makeMove(originRow,originCol,destRow,destCol);
    }

    //update move counter and move list widget
    moveList.addMove(originRow,originCol,destRow,destCol,game.getPlyCount());

    //check for game end or switch turn
    //if (move.isCheckmate()) setGameEnded(-game.turnColor());
    //if (move.isStalemate()) setGameEnded(bw::White | bw::Black);
    if (!game.ended()) switchTurn();
}

void boardMaster::networkMoveMake(int row1, int col1, int row2, int col2, int whiteTime, int blackTime, Piece promotionChoice)
{
    game.setTime(whiteTime, blackTime);
    game.startClock(); //this just means an unnecessary stop

    //now an ugly way to say: if we already made the move on the board,
    //we don't care what the client sent
    if (game.getPosition()({row1,col1}).piece==Piece::None) return;

    completeMove move(game.getPosition(),{{row1, col1}, {row2, col2}});

    board.moveMake(move); //update view
    game.setPosition(move.getNewBoard()); //update model

    if (promotionChoice != Piece::None){        
        toPromote.square_1.row = row1;
        toPromote.square_1.col = col1;
        toPromote.square_2.row = row2;
        toPromote.square_2.col = col2;
        promotionChoiceMade(promotionChoice);
    }

    //handle promotion AND update the engine, depending on whether it was or not
//    if (game.getPosition().wasPromotion){
//        handlePromotion(destRow, destCol);
//        if (!game.userBoth()) chessAi.makeMove(originRow,originCol,destRow,destCol, promotionChoice);
//    }else{
//        if (!game.userBoth()) chessAi.makeMove(originRow,originCol,destRow,destCol);
//    }

    //update move counter and move list widget
    moveList.addMove(row1,col1,row2,col2,game.getPlyCount());

    //check for game end or switch turn
    //if (move.isCheckmate()) setGameEnded(-game.turnColor());
    //if (move.isStalemate()) setGameEnded(bw::White | bw::Black);
    if (!game.ended()) switchTurn();

}

void boardMaster::newGame(Color whoUser, int time, std::string p1, std::string p2)
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

void boardMaster::aiTurn()
{
    Move moveToMake = chessAi.getMove();

    completeMove toCheck(game.getPosition(), moveToMake);
    BOOST_ASSERT_MSG(toCheck.isLegal(), "Engine tries to play illegal move");

    moveMake(toCheck);
}

void boardMaster::promotionChoiceMade(Piece whichPiece)
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

void boardMaster::promoteQueen()
{
    promotionChoiceMade(Piece::Queen);
}

void boardMaster::promoteBishop()
{
    promotionChoiceMade(Piece::Bishop);
}

void boardMaster::promoteKnight()
{
    promotionChoiceMade(Piece::Knight);
}

void boardMaster::promoteRook()
{
    promotionChoiceMade(Piece::Rook);
}

void boardMaster::whiteNewGame()
{
    sideChoice.enable(false);
    enableWindow(true);
    newGame(Color::White, 300, "White", "Black");
}

void boardMaster::blackNewGame()
{
    sideChoice.enable(false);
    enableWindow(true);
    newGame(Color::Black, 300, "White", "Black");
}

void boardMaster::bothNewGame()
{
    sideChoice.enable(false);
    enableWindow(true);
    newGame(Color::Both, 300, "White", "Black");
}

bool boardMaster::requestMove(const Move& move)
{
    if (!game.userTurn()) //set premove
    {
        premove = move;
        board.setArrow(move);
        return false;
    }

    completeMove toCheck(game.getPosition(), move);
    if (toCheck.isLegal()){
        moveMake(toCheck);
//        if (!toCheck.getNewBoard().wasPromotion) //pass to client only if it wasn't promotion
//            fics.makeMove(row1, col1, row2, col2);
        return true;
    }
    return false;
}

boardMaster::boardMaster(sf::Window &theWindow, sfg::Desktop &theDesktop):    
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
    board.getSignal().connect(boost::bind(&boardMaster::requestMove, this,_1));
    settingsWindow.settingsDone.connect(boost::bind(&boardMaster::settingsDone, this,_1,_2,_3));
    fics.positionReady.connect(boost::bind(&boardMaster::networkMoveMake, this, _1, _2, _3, _4, _5, _6, _7));
    fics.startGame.connect(boost::bind(&boardMaster::newGame, this, _1, _2, _3, _4));
    fics.gameEnd.connect(boost::bind(&boardMaster::setGameEnded, this, _1));
    fics.textReady.connect(boost::bind(&netWidgets::addLine, &netWindow, _1));
    netWindow.sendText.connect(boost::bind(&client::toClient, &fics, _1));

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

    queenButton->GetSignal(sfg::Widget::OnLeftClick).Connect(&boardMaster::promoteQueen,this);
    bishopButton->GetSignal(sfg::Widget::OnLeftClick).Connect(&boardMaster::promoteBishop,this);
    knightButton->GetSignal(sfg::Widget::OnLeftClick).Connect(&boardMaster::promoteKnight,this);
    rookButton->GetSignal(sfg::Widget::OnLeftClick).Connect(&boardMaster::promoteRook,this);

    promotionWindow->Show(false);

    buttonBox buttons;
    //buttons.resign()->GetSignal(sfg::Button::OnLeftClick).Connect(&boardMaster::resign, this);
    //buttons.draw()->GetSignal(sfg::Button::OnLeftClick).Connect(&boardMaster::offerDraw, this);
    //buttons.newGame()->GetSignal(sfg::Button::OnLeftClick).Connect(&boardMaster::requestNewGame, this);
    buttons.flip()->GetSignal(sfg::Button::OnLeftClick).Connect(&boardCanvas::flipBoard, &board);
    buttons.settings()->GetSignal(sfg::Button::OnLeftClick).Connect(&boardMaster::settingsClicked, this);

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
    sideChoice.getWhiteSide()->GetSignal(sfg::Button::OnLeftClick).Connect(&boardMaster::whiteNewGame, this);
    sideChoice.getBlackSide()->GetSignal(sfg::Button::OnLeftClick).Connect(&boardMaster::blackNewGame, this);
    sideChoice.getBothSide()->GetSignal(sfg::Button::OnLeftClick).Connect(&boardMaster::bothNewGame, this);

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

    //VALGRIND_CHECK_MEM_IS_DEFINED;
    //VALGRIND_CHECK_VALUE_IS_DEFINED(game);



}

boardMaster::~boardMaster()

{
    if (!game.userBoth()) chessAi.unLoad();
}

void boardMaster::display() //should rename to update
{
    //fics.update();
    board.display();    
    if (!game.ended()) updateClocks();
}

void boardMaster::switchTurn()
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


void boardMaster::resign()
{
    setGameEnded(!game.getUserColor()); //should be turncolor
}

void boardMaster::offerDraw()
{
    return; //ai rejects all offers for now
}


void boardMaster::requestNewGame()
{
    sideChoice.enable(true);
    enableWindow(false);
}

void boardMaster::updateClocks()
{
    game.update(); //update model
    clocks.update(game.getWhiteTime(), game.getBlackTime()); //update view
}
