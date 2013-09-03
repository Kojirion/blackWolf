#include "boardmaster.h"
#include <boost/assert.hpp>
#include <SFGUI/Box.hpp>
#include <SFGUI/ScrolledWindow.hpp>
#include "components/buttonbox.h"

void boardMaster::setGameEnded(bw result)
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

void boardMaster::flagDown(const bw loser)
{
    clocks.setFlagDown(loser);
    setGameEnded(-loser);
}

void boardMaster::handlePromotion(const int row, const int col)
{
    toPromoteRow = row;
    toPromoteCol = col;

    if (game.userTurn()){
        //enable promotion window
        promotionWindow->Show(true);
        desktop.BringToFront(promotionWindow);

        //disable board window
        enableWindow(false);
    }else{
        promotionChoiceMade(chessAi.getPromotionChoice());
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
    game.setPosition(move.getNewBoard()); //update model

    //handle promotion AND update the engine, depending on whether it was or not
    if (game.getPosition().wasPromotion){
        handlePromotion(destRow, destCol);
         if (!game.userBoth()) chessAi.makeMove(originRow,originCol,destRow,destCol, promotionChoice);
    }else{
         if (!game.userBoth()) chessAi.makeMove(originRow,originCol,destRow,destCol);
    }

    //update move counter and move list widget
    moveList.addMove(originRow,originCol,destRow,destCol,game.getPlyCount());

    //check for game end or switch turn
    if (move.isCheckmate()) setGameEnded(-game.turnColor());
    if (move.isStalemate()) setGameEnded(bw::White | bw::Black);
    if (!game.ended()) switchTurn();
}

void boardMaster::networkMoveMake(int row1, int col1, int row2, int col2, int whiteTime, int blackTime)
{
    game.setTime(whiteTime, blackTime);

    //now an ugly way to say: if we already made the move on the board,
    //we don't care what the client sent
    if (game.getPosition()[row1][col1]==0) return;

    completeMove move(game.getPosition(),row1, col1, row2, col2);

    board.moveMake(move); //update view
    game.setPosition(move.getNewBoard()); //update model

    //handle promotion AND update the engine, depending on whether it was or not
//    if (game.getPosition().wasPromotion){
//        handlePromotion(destRow, destCol);
//        if (!game.userBoth()) chessAi.makeMove(originRow,originCol,destRow,destCol, promotionChoice);
//    }else{
//        if (!game.userBoth()) chessAi.makeMove(originRow,originCol,destRow,destCol);
//    }

    //update move counter and move list widget
    //moveList.addMove(originRow,originCol,destRow,destCol,game.getPlyCount());

    //check for game end or switch turn
    //if (move.isCheckmate()) setGameEnded(-game.turnColor());
    //if (move.isStalemate()) setGameEnded(bw::White | bw::Black);
    if (!game.ended()) switchTurn();

}

void boardMaster::newGame(const bw whoUser, int time)
{
    game.newGame(whoUser, time);

    board.resetFor(whoUser);

    board.setPosition(game.getPosition());

    moveList.reset();

    status.setToPlay(bw::White);

    updateClocks();

    //if (!game.userBoth()) chessAi.newGame();

    //if (!game.userTurn()) aiTurn();

}

void boardMaster::aiTurn()
{
    chessEngine::move moveToMake = chessAi.getMove();
    const int originRow = std::get<0>(moveToMake);
    const int originCol = std::get<1>(moveToMake);
    const int destRow = std::get<2>(moveToMake);
    const int destCol = std::get<3>(moveToMake);

    completeMove toCheck(game.getPosition(),originRow,originCol,destRow,destCol);
    BOOST_ASSERT_MSG(toCheck.isLegal(), "Engine tries to play illegal move");

    moveMake(toCheck);
}

void boardMaster::promotionChoiceMade(const int whichPiece)
{
    promotionChoice = whichPiece;

    //update view
    board.setPromotion(toPromoteRow, toPromoteCol, whichPiece);

    //update model
    int whichSide;
    if (game.turnColor()==bw::White) whichSide = 1;
    else whichSide = 1;
    game.setPromotion(toPromoteRow,toPromoteCol,whichPiece*whichSide);

    promotionWindow->Show(false);
    enableWindow(true);
}

void boardMaster::promoteQueen()
{
    promotionChoiceMade(4);
}

void boardMaster::promoteBishop()
{
    promotionChoiceMade(2);
}

void boardMaster::promoteKnight()
{
    promotionChoiceMade(3);
}

void boardMaster::promoteRook()
{
    promotionChoiceMade(1);
}

void boardMaster::whiteNewGame()
{
    sideChoice.enable(false);
    enableWindow(true);
    newGame(bw::White, 300);
}

void boardMaster::blackNewGame()
{
    sideChoice.enable(false);
    enableWindow(true);
    newGame(bw::Black, 300);
}

void boardMaster::bothNewGame()
{
    sideChoice.enable(false);
    enableWindow(true);
    newGame(bw::White | bw::Black, 300);
}

bool boardMaster::requestMove(int row1, int col1, int row2, int col2)
{
    if (!game.userTurn()) //set premove
    {
        premove = std::make_tuple(true,row1,col1,row2,col2);
        board.setArrow(row1,col1,row2,col2);
        return false;
    }

    completeMove toCheck(game.getPosition(),row1,col1,row2,col2);
    if (toCheck.isLegal()){
        moveMake(toCheck);
        fics.makeMove(row1, col1, row2, col2);
        return true;
    }
    return false;
}

boardMaster::boardMaster(sf::Window &theWindow, sfg::Desktop &theDesktop):
    promotionWindow(sfg::Window::Create()),
    boardWindow(sfg::Window::Create()),
    toPromoteRow(0), toPromoteCol(0), promotionChoice(0),
    desktop(theDesktop),
    settingsButton(sfg::Button::Create("Settings")),
    board(theWindow,resources),
    sideChoice(desktop),
    settingsWindow(desktop),
    premove(std::make_tuple(false,0,0,0,0))
{
    board.getSignal().connect(boost::bind(&boardMaster::requestMove, this,_1,_2,_3,_4));
    settingsWindow.settingsDone.connect(boost::bind(&boardMaster::settingsDone, this,_1,_2,_3));
    fics.positionReady.connect(boost::bind(&boardMaster::networkMoveMake, this, _1, _2, _3, _4, _5, _6));
    fics.startGame.connect(boost::bind(&boardMaster::newGame, this, _1, _2));
    fics.gameEnd.connect(boost::bind(&boardMaster::setGameEnded, this, _1));

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
    buttons.resign()->GetSignal(sfg::Button::OnLeftClick).Connect(&boardMaster::resign, this);
    buttons.draw()->GetSignal(sfg::Button::OnLeftClick).Connect(&boardMaster::offerDraw, this);
    buttons.newGame()->GetSignal(sfg::Button::OnLeftClick).Connect(&boardMaster::requestNewGame, this);
    buttons.flip()->GetSignal(sfg::Button::OnLeftClick).Connect(&boardCanvas::flipBoard, &board);
    buttons.settings()->GetSignal(sfg::Button::OnLeftClick).Connect(&boardMaster::settingsClicked, this);

    sfg::Table::Ptr mainLayout(sfg::Table::Create());
    mainLayout->SetRowSpacings(5.f);
    mainLayout->Attach(board.getBoardWidget(),{0, 0, 1, 8},sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 10.f, 0.f ));
    mainLayout->Attach(clocks.getWhiteClock(),{1, 0, 1, 1});
    mainLayout->Attach(clocks.getBlackClock(),{1, 1, 1, 1});
    mainLayout->Attach(status.getView(),{1, 2, 1, 1});
    mainLayout->Attach(moveList.getView(),{1, 3, 1, 4});
    mainLayout->Attach(buttons.getWidget(),{0,8,2,2});

    //when making new game
    sideChoice.getWhiteSide()->GetSignal(sfg::Button::OnLeftClick).Connect(&boardMaster::whiteNewGame, this);
    sideChoice.getBlackSide()->GetSignal(sfg::Button::OnLeftClick).Connect(&boardMaster::blackNewGame, this);
    sideChoice.getBothSide()->GetSignal(sfg::Button::OnLeftClick).Connect(&boardMaster::bothNewGame, this);

    desktop.Add(sideChoice.getWidget());
    desktop.Add(settingsWindow.getWidget());







    boardWindow->Add(mainLayout);


    desktop.Add(promotionWindow);
    desktop.Add(boardWindow);

    fics.connect();
    //newGame(bw::White);

//    if (!chessAi.load()) newGame(bw::White | bw::Black);
//    else newGame(bw::White);



}

boardMaster::~boardMaster()

{
    if (!game.userBoth()) chessAi.unLoad();
}

void boardMaster::display() //should rename to update
{
    fics.update();
    board.display();    
    if (!game.ended()) updateClocks();
}

void boardMaster::switchTurn()
{
    status.setToPlay(game.turnColor());
    game.switchTurn();

    if (std::get<0>(premove))
    {
        std::get<0>(premove) = false;
        board.clearArrows();
        requestMove(std::get<1>(premove),std::get<2>(premove),std::get<3>(premove),std::get<4>(premove));
    }

//    if (!game.userTurn()){
//        aiTurn();
//    }
}


void boardMaster::resign()
{
    setGameEnded(-game.getUserColor());
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
