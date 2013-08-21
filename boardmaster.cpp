#include "boardmaster.h"
#include <sstream>
#include <boost/assert.hpp>
#include <SFGUI/Box.hpp>
#include <SFGUI/ScrolledWindow.hpp>

/*void boardMaster::setGameEnded(const int result)
{
    gameEnded = true;

    switch (result) {
    case 1:
        turnLabel_->SetText("White wins!");
        break;
    case -1:
        turnLabel_->SetText("Black wins!");
        break;
    case 0:
        turnLabel_->SetText("Draw!");
        break;
    }

}*/

void boardMaster::flagDown(const int side)
{
    //if (side==1) whiteClockText.setColor(sf::Color::Yellow);
    //else blackClockText.setColor(sf::Color::Yellow);
    //setGameEnded(-side);
}

void boardMaster::handlePromotion(const int row, const int col)
{
    toPromoteRow = row;
    toPromoteCol = col;

    if (game.userTurn()){
        promotionWindow->Show(true);
        desktop.BringToFront(promotionWindow);
        boardWindow->SetState(sfg::Widget::INSENSITIVE);
    }else{
        promotionChoiceMade(chessAi.getPromotionChoice());
    }
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
         //if (!humanBoth) chessAi.makeMove(originRow,originCol,destRow,destCol, promotionChoice);
    }else{
         //if (!humanBoth) chessAi.makeMove(originRow,originCol,destRow,destCol);
    }

    //update move counter and move list widget
    moveList.addMove(originRow,originCol,destRow,destCol,game.getPlyCount());

    //check for game end or switch turn
    if (move.isCheckmate()) game.setResult(-game.turnColor());
    if (move.isStalemate()) game.setResult(bw::White | bw::Black);
    if (!game.ended()) switchTurn();
}

void boardMaster::newGame(const int whoHuman)
{
    //moveList->RemoveAll();
    status.setToPlay(bw::White);

    /*if (flipped()){
        if (humanColor==1) flipBoard();
    }else{
        if (humanColor==-1) flipBoard();
    }*/

    //resetRects();

    //idCount = 1;
    //pieces.clear();

    //initPieces();

    chessAi.newGame();

    if (!game.userTurn()) aiTurn();

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
    /*promotionChoice = whichPiece;
    const int whichSide = pieces[toPromoteRow][toPromoteCol].getSide();
    destroy(toPromoteRow,toPromoteCol);
    pieceSprite toAdd(resources.typeToTexture(whichSide*whichPiece),cellToPosition(toPromoteRow,toPromoteCol),whichSide,idCount);
    pieces[toPromoteRow][toPromoteCol].insert(toAdd);
    idCount++;
    currentPosition.setPromotion(toPromoteRow,toPromoteCol,whichPiece*whichSide);

    promotionWindow->Show(false);
    boardWindow->SetState(sfg::Widget::NORMAL);*/
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
    sideChoiceWindow->Show(false);
    boardWindow->SetState(sfg::Widget::NORMAL);
    newGame(1);
}

void boardMaster::blackNewGame()
{
    sideChoiceWindow->Show(false);
    boardWindow->SetState(sfg::Widget::NORMAL);
    newGame(-1);
}

void boardMaster::bothNewGame()
{
    sideChoiceWindow->Show(false);
    boardWindow->SetState(sfg::Widget::NORMAL);
    newGame(2);
}

bool boardMaster::requestMove(int row1, int col1, int row2, int col2)
{
    if (!game.userTurn()) return false;

    completeMove toCheck(game.getPosition(),row1,col1,row2,col2);
    if (toCheck.isLegal()){
        moveMake(toCheck);
        return true;
    }
    return false;
}

boardMaster::boardMaster(sf::Window &theWindow, sfg::Desktop &theDesktop):
    promotionWindow(sfg::Window::Create()), sideChoiceWindow(sfg::Window::Create()),
    boardWindow(sfg::Window::Create()),
    toPromoteRow(0), toPromoteCol(0), promotionChoice(0),
    desktop(theDesktop),
    settingsButton(sfg::Button::Create("Settings")),
    board(theWindow,resources)
{
    //if (!chessAi.load()) humanBoth = true;
    //humanBoth = true;

    board.getSignal().connect(boost::bind(&boardMaster::requestMove, this,_1,_2,_3,_4));

    sfg::Button::Ptr queenButton(sfg::Button::Create("Queen"));
    sfg::Button::Ptr bishopButton(sfg::Button::Create("Bishop"));
    sfg::Button::Ptr knightButton(sfg::Button::Create("Knight"));
    sfg::Button::Ptr rookButton(sfg::Button::Create("Rook"));

    sfg::Box::Ptr promotionBox(sfg::Box::Create(sfg::Box::HORIZONTAL, 3.f));

    promotionBox->Pack(queenButton);
    promotionBox->Pack(bishopButton);
    promotionBox->Pack(knightButton);
    promotionBox->Pack(rookButton);

    promotionWindow->Add(promotionBox);
    promotionWindow->SetPosition(sf::Vector2f(200.f,200.f));
    promotionWindow->SetTitle("Choose piece");

    queenButton->GetSignal(sfg::Widget::OnLeftClick).Connect(&boardMaster::promoteQueen,this);
    bishopButton->GetSignal(sfg::Widget::OnLeftClick).Connect(&boardMaster::promoteBishop,this);
    knightButton->GetSignal(sfg::Widget::OnLeftClick).Connect(&boardMaster::promoteKnight,this);
    rookButton->GetSignal(sfg::Widget::OnLeftClick).Connect(&boardMaster::promoteRook,this);

    promotionWindow->Show(false);

    //gui setup
    sfg::Button::Ptr resignButton(sfg::Button::Create("Resign"));
    resignButton->GetSignal(sfg::Button::OnLeftClick).Connect(&boardMaster::resign, this);

    sfg::Button::Ptr drawButton(sfg::Button::Create("Offer draw"));
    drawButton->GetSignal(sfg::Button::OnLeftClick).Connect(&boardMaster::offerDraw, this);

    sfg::Button::Ptr newGameButton(sfg::Button::Create("New game"));
    newGameButton->GetSignal(sfg::Button::OnLeftClick).Connect(&boardMaster::requestNewGame, this);

    sfg::Button::Ptr flipButton(sfg::Button::Create("Flip board"));
    //flipButton->GetSignal(sfg::Button::OnLeftClick).Connect(&boardMaster::flipBoard, this);

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
    mainLayout->Attach(board.getBoardWidget(),{0, 0, 1, 8},sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 10.f, 0.f ));
    mainLayout->Attach(clocks.getWhiteClock(),{1, 0, 1, 1});
    mainLayout->Attach(clocks.getBlackClock(),{1, 1, 1, 1});
    mainLayout->Attach(status.getView(),{1, 2, 1, 1});
    mainLayout->Attach(moveList.getView(),{1, 3, 1, 4});
    mainLayout->Attach(buttonLayout,{0,8,2,2});

    //when making new game
    sideChoiceWindow->SetPosition(sf::Vector2f(200.f,200.f));
    sideChoiceWindow->SetRequisition(sf::Vector2f(200.f,200.f));
    sideChoiceWindow->SetTitle("Choose color");

    sfg::Box::Ptr sideChoiceBox(sfg::Box::Create(sfg::Box::VERTICAL, 5.f));
    sfg::Button::Ptr whiteSide(sfg::Button::Create("White"));
    sfg::Button::Ptr blackSide(sfg::Button::Create("Black"));
    sfg::Button::Ptr bothSide(sfg::Button::Create("Both"));

    whiteSide->GetSignal(sfg::Button::OnLeftClick).Connect(&boardMaster::whiteNewGame, this);
    blackSide->GetSignal(sfg::Button::OnLeftClick).Connect(&boardMaster::blackNewGame, this);
    bothSide->GetSignal(sfg::Button::OnLeftClick).Connect(&boardMaster::bothNewGame, this);

    sideChoiceBox->Pack(whiteSide);
    sideChoiceBox->Pack(blackSide);
    sideChoiceBox->Pack(bothSide);
    sideChoiceWindow->Add(sideChoiceBox);
    sideChoiceWindow->Show(false);
    desktop.Add(sideChoiceWindow);

    status.setToPlay(bw::White);





    boardWindow->Add(mainLayout);


    desktop.Add(promotionWindow);
    desktop.Add(boardWindow);

    game.newGame(bw::White | bw::Black);

    board.setPosition(game.getPosition());


    updateClocks();





}

boardMaster::~boardMaster()
{
    //if (!humanBoth) chessAi.unLoad();
}

void boardMaster::display()
{
    board.display();
    if (!game.ended()) updateClocks();
}

int boardMaster::getTurnColor() const
{
    return 1; //currentPosition.turnColor;
}

void boardMaster::switchTurn()
{
    status.setToPlay(game.turnColor());
    game.switchTurn();

//    if (!game.userTurn()){
//        aiTurn();
//    }
}


void boardMaster::resign()
{
    game.setResult(-game.getUserColor());
}

void boardMaster::offerDraw()
{
    return; //ai rejects all offers for now
}

void boardMaster::requestNewGame()
{
    sideChoiceWindow->Show(true);
    desktop.BringToFront(sideChoiceWindow);
    boardWindow->SetState(sfg::Widget::INSENSITIVE);
}

void boardMaster::updateClocks()
{
    game.update(); //update model
    clocks.update(game.getWhiteTime(), game.getBlackTime()); //update view
}
