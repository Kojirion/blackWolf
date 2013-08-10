#include "boardmaster.h"
#include <sstream>
#include <boost/assert.hpp>

void boardMaster::setGameEnded(const int result)
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

}

bool boardMaster::pieceHeld()
{
    return (currentPiece.isValid());
}

void boardMaster::releasePiece()
{
    currentPiece.invalidate();
}

void boardMaster::flagDown(const int side)
{
    if (side==1) whiteClockText.setColor(sf::Color::Yellow);
    else blackClockText.setColor(sf::Color::Yellow);
    setGameEnded(-side);
}

void boardMaster::handleCastle(const int row, const int col)
{
    if (row==0){
        if (col==2){
            pieces[0][0].moveTo(0,3,cellToPosition(0,3));
        }else{
            BOOST_ASSERT_MSG(col==6, "Invalid Castle");
            pieces[0][7].moveTo(0,5,cellToPosition(0,5));
        }
    }else{
        if (col==2){
            BOOST_ASSERT_MSG(row==7, "Invalid Castle");
            pieces[7][0].moveTo(7,3,cellToPosition(7,3));
        }else{
            BOOST_ASSERT_MSG(col==6, "Invalid Castle");            
            pieces[7][7].moveTo(7,5,cellToPosition(7,5));
        }
    }
}

void boardMaster::handleEnPassant(const int row, const int col)
{
    if (row==5){
        destroy(4,col);
    }else{
        BOOST_ASSERT_MSG(row==2, "Invalid en passant");
        destroy(3,col);
    }
}

void boardMaster::handlePromotion(const int row, const int col)
{
    const int whichSide = pieces[row][col].getSide();
    destroy(row,col);
    pieceSprite toAdd(idToTexture(whichSide*4),cellToPosition(row,col),whichSide,idCount);
    pieces[row][col].insert(toAdd);
    idCount++;
}

void boardMaster::moveMake(const completeMove &move)
{
    const int originRow = move.getRow1();
    const int originCol = move.getCol1();
    const int destRow = move.getRow2();
    const int destCol = move.getCol2();

    if (!humanBoth) chessAi.makeMove(originRow,originCol,destRow,destCol); //update engine's internal board
    destroy(destRow,destCol); //destroy any sprites at destination
    pieces[originRow][originCol].moveTo(destRow, destCol, cellToPosition(destRow, destCol));

    releasePiece(); //resets the currentPiece iterator
    currentPosition = move.getNewBoard(); //set currentPosition to the new board of the move
    if (currentPosition.wasCastle) handleCastle(destRow,destCol);
    if (currentPosition.wasEnPassant) handleEnPassant(destRow,destCol);
    if (currentPosition.wasPromotion) handlePromotion(destRow, destCol);

    //update move counter and move list widget
    sfg::Label::Ptr newMove(sfg::Label::Create(moveToString(originRow,originCol,destRow,destCol)));
    const int plyPairsCount = plyCounter/2;
    const int plyRemainder = (plyCounter)%2;
    moveList->Attach(newMove,{plyRemainder,plyPairsCount,1,1});
    plyCounter++;

    //check for game end or switch turn
    if (move.isCheckmate()) setGameEnded(-getTurnColor());
    if (move.isStalemate()) setGameEnded(0);
    if (!gameEnded) switchTurn();
}

void boardMaster::destroy(const int row, const int col)
{
    pieces[row][col].erase();
}

boardMaster::boardMaster(sf::Window &theWindow):
    flipOffset(0,0),
    window_(sfg::Canvas::Create()),
    bigWindow(theWindow),
    turnLabel_(sfg::Label::Create("White to play")),
    whiteClockCanvas_(sfg::Canvas::Create()),
    blackClockCanvas_(sfg::Canvas::Create()),
    moveList(sfg::Table::Create()),
    plyCounter(0),
    humanColor(1),
    humanBoth(false),
    gameEnded(false),
    currentPiece(&pieces),
    idCount(1)
{
    releasePiece();

    if (!chessAi.load()) humanBoth = true;
    //humanBoth = true;

    whiteClock.connect(std::bind(&boardMaster::flagDown, this, 1));
    blackClock.connect(std::bind(&boardMaster::flagDown, this, -1));

    sfg::Label::Ptr dummyLabel(sfg::Label::Create());
    moveList->SetColumnSpacings(0.f);
    moveList->Attach(dummyLabel,{0,0,1,1});
    moveList->SetColumnSpacing(0,10.f);
    moveList->Remove(dummyLabel);

    window_->SetRequisition(sf::Vector2f( 440.f, 440.f ));
    window_->GetSignal(sfg::Widget::OnMouseLeftPress).Connect(&boardMaster::processLeftClick, this);
    window_->GetSignal(sfg::Widget::OnMouseMove).Connect(&boardMaster::processMouseMove, this);
    window_->GetSignal(sfg::Widget::OnMouseLeftRelease).Connect(&boardMaster::processMouseRelease, this);
    window_->GetSignal(sfg::Widget::OnMouseEnter).Connect(&boardMaster::processEnterCanvas, this);

    font.loadFromFile("DejaVuSans.ttf"); //assert it


    boardTexture_.loadFromFile("Graphics/Boardbrown.jpg");
    boardSprite_.setTexture(boardTexture_);    

    blackRookT.loadFromFile("Graphics/Pieces/BlackR.png");
    blackBishopT.loadFromFile("Graphics/Pieces/BlackB.png");
    blackKnightT.loadFromFile("Graphics/Pieces/BlackN.png");
    blackQueenT.loadFromFile("Graphics/Pieces/BlackQ.png");
    blackKingT.loadFromFile("Graphics/Pieces/BlackK.png");
    blackPawnT.loadFromFile("Graphics/Pieces/BlackP.png");
    whiteRookT.loadFromFile("Graphics/Pieces/WhiteR.png");
    whiteBishopT.loadFromFile("Graphics/Pieces/WhiteB.png");
    whiteKnightT.loadFromFile("Graphics/Pieces/WhiteN.png");
    whiteQueenT.loadFromFile("Graphics/Pieces/WhiteQ.png");
    whiteKingT.loadFromFile("Graphics/Pieces/WhiteK.png");
    whitePawnT.loadFromFile("Graphics/Pieces/WhiteP.png");

    for (int i=0; i<8; ++i){
        for (int j=0; j<8; ++j){
            const int pieceId = currentPosition[i][j];
            if (pieceId==0) continue;
            pieceSprite toAdd(idToTexture(pieceId),cellToPosition(i,j),pieceId, idCount);
            pieces[i][j].insert(toAdd);
            idCount++;
        }
    }

    rectGrid.resize(8);
    for (int i=0; i<8; ++i){
        rectGrid[i].resize(8);
        for (int j=0; j<8; ++j){
            sf::Vector2f toSet = cellToPosition(i,j);
            rectGrid[i][j].left = toSet.x;
            rectGrid[i][j].top = toSet.y;
            rectGrid[i][j].width = 50.f;
            rectGrid[i][j].height = 50.f;
        }
    }

    whiteClockText.setFont(font);
    whiteClockText.setCharacterSize(20);
    //whiteClockText.setPosition(0.f, 0.f);
    whiteClockText.setColor(sf::Color(0, 140, 190));

    blackClockText.setFont(font);
    blackClockText.setCharacterSize(20);
    //blackClockText.setPosition(70.f, 150.f);
    blackClockText.setColor(sf::Color(0, 140, 190));

    whiteClockCanvas_->SetRequisition(sf::Vector2f(100,50));
    blackClockCanvas_ = sfg::Canvas::Create();
    blackClockCanvas_->SetRequisition(sf::Vector2f(100,50));

    whiteClock.restart(sf::seconds(300));
    blackClock.restart(sf::seconds(300));
    blackClock.stop();


    updateClocks();





}

boardMaster::~boardMaster()
{
    if (!humanBoth) chessAi.unLoad();
}

void boardMaster::display()
{
    window_->Clear();

    window_->Draw(boardSprite_);

    for (auto &piece : pieces){
        window_->Draw(piece);
    }
    if (!gameEnded) updateClocks();

    //window_->Display();

}

sf::Vector2f boardMaster::cellToPosition(const int row, const int col) const
{
    return sf::Vector2f(flipOffset.x * (7 - 2*col) + 20 + 50 * col, -flipOffset.y * (7 - 2*row) + 420 - 50 * (row+1));
}

const sf::Texture &boardMaster::idToTexture(const int pieceId) const
{
    switch (pieceId) {
    case 1:
        return whiteRookT;
    case 2:
        return whiteBishopT;
    case 3:
        return whiteKnightT;
    case 4:
        return whiteQueenT;
    case 5:
        return whitePawnT;
    case 6:
        return whiteKingT;
    case -1:
        return blackRookT;
    case -2:
        return blackBishopT;
    case -3:
        return blackKnightT;
    case -4:
        return blackQueenT;
    case -5:
        return blackPawnT;
    case -6:
        return blackKingT;
    }
}

sf::Vector2f boardMaster::getMousePosition()
{
    return (static_cast<sf::Vector2f>(sf::Mouse::getPosition(bigWindow)) - window_->GetAbsolutePosition());
}

int boardMaster::getTurnColor() const
{
    return currentPosition.turnColor;
}

void boardMaster::switchTurn()
{
    if (getTurnColor() == 1){
        blackClock.stop();
        whiteClock.start();
        turnLabel_->SetText("White to play");
    }else{
        whiteClock.stop();
        blackClock.start();
        turnLabel_->SetText("Black to play");
    }


    if (humanBoth) {
        humanColor = getTurnColor();
    }
    else if (humanColor != getTurnColor()){
        chessEngine::move moveToMake = chessAi.getMove();
        const int originRow = std::get<0>(moveToMake);
        const int originCol = std::get<1>(moveToMake);
        const int destRow = std::get<2>(moveToMake);
        const int destCol = std::get<3>(moveToMake);

        completeMove toCheck(currentPosition,originRow,originCol,destRow,destCol);
        BOOST_ASSERT_MSG(toCheck.isLegal(), "Engine tries to play illegal move");

        moveMake(toCheck);
    }
}


    void boardMaster::sendBack()
    {
        BOOST_ASSERT_MSG(pieceHeld(), "No current piece to send back");

        pieces[currentPiece].sendTo(cellToPosition(currentPiece.getRow(),currentPiece.getCol()));
        
        releasePiece();
    }

    void boardMaster::processLeftClick()
    {
        if (gameEnded) return;
        if (humanColor != getTurnColor()) return; //must alter this for pre-move in future

        clickedPoint = getMousePosition();

        const int whoseTurn = getTurnColor();

        for (auto &piece : pieces){
            if (piece.getSide()!=whoseTurn) continue;
            if (piece.contains(clickedPoint)){
                currentPiece = pieces.spriteToIt(piece);
                break;
            }
        }
    }

void boardMaster::processMouseMove()
{
    if (pieceHeld()){
        pieces[currentPiece].sendTo(getMousePosition()-sf::Vector2f(25.f,25.f));
    }
}

void boardMaster::processMouseRelease()
{
    if (pieceHeld()){
        sf::Vector2f centrePos = pieces[currentPiece].getPosition() + sf::Vector2f(25.f,25.f);
        for (int i=0; i<8; ++i){
            for (int j=0; j<8; ++j){
                if (rectGrid[i][j].contains(centrePos)){
                    const int originRow = currentPiece.getRow();
                    const int originCol = currentPiece.getCol();
                    completeMove toCheck(currentPosition,originRow,originCol,i,j);
                    if (toCheck.isLegal()){
                        moveMake(toCheck);
                    }else{
                        sendBack();
                    }
                    return;
                }
            }
        }
        sendBack();
    }
}

void boardMaster::processEnterCanvas()
{
    if (pieceHeld()){
        if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) sendBack();
    }
}

void boardMaster::flipBoard()
{
    if (flipOffset!=sf::Vector2f()) flipOffset = sf::Vector2f();
    else flipOffset = sf::Vector2f(50.f,50.f);

    for (auto &piece : pieces){
        auto toFlip = pieces[piece];
        toFlip.sendTo(cellToPosition(toFlip.getRow(), toFlip.getCol()));
    }
}

std::string boardMaster::toString(sf::Time value) const
{
    int minutes = value.asSeconds()/60;
    int seconds = static_cast<int>(value.asSeconds())%60;

    std::ostringstream stream;
    stream.setf(std::ios_base::fixed);
    stream.precision(2);
    stream << minutes << ":";
    if (seconds<10) stream << "0";
    stream << seconds;
    return stream.str();
}

std::string boardMaster::colToString(const int col) const
{
    switch (col) {
    case 0:
        return "a";
    case 1:
        return "b";
    case 2:
        return "c";
    case 3:
        return "d";
    case 4:
        return "e";
    case 5:
        return "f";
    case 6:
        return "g";
    case 7:
        return "h";
    }
}

std::string boardMaster::moveToString(const int row1, const int col1, const int row2, const int col2) const
{
    return (cellToString(row1,col1) + "-" + cellToString(row2,col2));
}

std::string boardMaster::cellToString(const int row, const int col) const
{
    return (colToString(col) + std::to_string(row+1));
}

void boardMaster::updateClocks()
{
    whiteClock.update();
    whiteClockText.setString(toString(whiteClock.getRemainingTime()));
    whiteClockCanvas_->Clear();
    whiteClockCanvas_->Draw(whiteClockText);

    blackClock.update();
    blackClockText.setString(toString(blackClock.getRemainingTime()));
    blackClockCanvas_->Clear();
    blackClockCanvas_->Draw(blackClockText);    
}
