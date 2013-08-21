#include "boardcanvas.h"

boardCanvas::boardCanvas(sf::Window& theWindow, resourceManager& theResources):
    flipOffset(0),
    window(sfg::Canvas::Create()),
    bigWindow(theWindow),
    currentPiece(&pieces),
    idCount(1),
    resources(theResources)
{
    boardTexture_.loadFromFile("Graphics/Boardbrown.jpg");
    boardSprite_.setTexture(boardTexture_);

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

    window->SetRequisition(sf::Vector2f( 440.f, 440.f ));
    window->GetSignal(sfg::Widget::OnMouseLeftPress).Connect(&boardCanvas::slotLeftClick, this);
    window->GetSignal(sfg::Widget::OnMouseMove).Connect(&boardCanvas::slotMouseMove, this);
    window->GetSignal(sfg::Widget::OnMouseLeftRelease).Connect(&boardCanvas::slotMouseRelease, this);
    window->GetSignal(sfg::Widget::OnMouseEnter).Connect(&boardCanvas::slotEnterCanvas, this);
}

bool boardCanvas::flipped() const
{
    return (flipOffset!=0);
}

bool boardCanvas::pieceHeld()
{
    return (currentPiece.isValid());
}

void boardCanvas::releasePiece()
{
    currentPiece.invalidate();
}

void boardCanvas::destroy(const int row, const int col)
{
    pieces[row][col].erase();
}

void boardCanvas::display()
{
    window->Clear();

    window->Draw(boardSprite_);

    for (auto& piece : pieces){
        window->Draw(piece);
    }
}

void boardCanvas::moveMake(const completeMove move)
{
    const int originRow = move.getRow1();
    const int originCol = move.getCol1();
    const int destRow = move.getRow2();
    const int destCol = move.getCol2();

    destroy(destRow,destCol); //destroy any sprites at destination
    pieces[originRow][originCol].moveTo(destRow, destCol, cellToPosition(destRow, destCol));

    releasePiece(); //resets the currentPiece iterator
    position currentPosition = move.getNewBoard();
    if (currentPosition.wasCastle) handleCastle(destRow,destCol);
    if (currentPosition.wasEnPassant) handleEnPassant(destRow,destCol);
}

sfg::Widget::Ptr boardCanvas::getBoardWidget() const
{
    return window;
}

sf::Vector2f boardCanvas::cellToPosition(const int row, const int col) const
{
    return sf::Vector2f(flipOffset * (7 - 2*col) + 20 + 50 * col, -flipOffset * (7 - 2*row) + 420 - 50 * (row+1));
}

sf::Vector2f boardCanvas::getMousePosition() const
{
    return (static_cast<sf::Vector2f>(sf::Mouse::getPosition(bigWindow)) - window->GetAbsolutePosition());
}

void boardCanvas::sendBack()
{
    BOOST_ASSERT_MSG(pieceHeld(), "No current piece to send back");

    pieces[currentPiece].sendTo(cellToPosition(currentPiece.getRow(),currentPiece.getCol()));

    releasePiece();
}

void boardCanvas::flipBoard()
{
    if (flipped()) flipOffset = 0;
    else flipOffset = 50;

    for (auto &piece : pieces){
        auto toFlip = pieces[piece];
        toFlip.sendTo(cellToPosition(toFlip.getRow(), toFlip.getCol()));
    }

    resetRects();
}

void boardCanvas::handleCastle(const int row, const int col)
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

void boardCanvas::handleEnPassant(const int row, const int col)
{
    if (row==5){
        destroy(4,col);
    }else{
        BOOST_ASSERT_MSG(row==2, "Invalid en passant");
        destroy(3,col);
    }
}

void boardCanvas::slotLeftClick()
{
    //if (gameEnded) return; - it should be disabled from outside
    //if (humanColor != getTurnColor()) return; - boardMaster will decide that

    sf::Vector2f clickedPoint = getMousePosition();

    //const int whoseTurn = getTurnColor();

    for (auto &piece : pieces){
        //if (piece.getSide()!=whoseTurn) continue;
        if (piece.contains(clickedPoint)){
            currentPiece = pieces.spriteToIt(piece);
            break;
        }
    }
}

void boardCanvas::slotMouseMove()
{
    if (pieceHeld()){
        pieces[currentPiece].sendTo(getMousePosition()-sf::Vector2f(25.f,25.f));
    }
}

void boardCanvas::slotMouseRelease()
{
    if (pieceHeld()){
        sf::Vector2f centrePos = pieces[currentPiece].getPosition() + sf::Vector2f(25.f,25.f);
        for (int i=0; i<8; ++i){
            for (int j=0; j<8; ++j){
                if (rectGrid[i][j].contains(centrePos)){
                    const int originRow = currentPiece.getRow();
                    const int originCol = currentPiece.getCol();
                    //send request move signal to controller
                    if (!(*requestMove(originRow, originCol, i, j)))
                        sendBack(); //if rejected send piece back
                    return;
                }
            }
        }
        sendBack();
    }
}

void boardCanvas::slotEnterCanvas()
{
    if (pieceHeld()){
        if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) sendBack();
    }
}

void boardCanvas::setPosition(const position& givenPosition)
{
    for (int i=0; i<8; ++i){
        for (int j=0; j<8; ++j){
            const int pieceId = givenPosition[i][j];
            if (pieceId==0) continue;
            pieceSprite toAdd(resources.typeToTexture(pieceId),cellToPosition(i,j),pieceId, idCount);
            pieces[i][j].insert(toAdd);
            idCount++;
        }
    }

}

void boardCanvas::resetFor(const bw whoFaceUp)
{
    if (flipped()){
        if (check(whoFaceUp & bw::White)) flipBoard();
    }else{
        if (!check(whoFaceUp & bw::White)) flipBoard();
    }

    resetRects();

    idCount = 1;
    pieces.clear();
}

boost::signals2::signal<bool (int, int, int, int)>& boardCanvas::getSignal()
{
    return requestMove;
}

void boardCanvas::setPromotion(const int row, const int col, const int piece)
{
    const int whichSide = pieces[row][col].getSide();
    destroy(row,col);
    pieceSprite toAdd(resources.typeToTexture(whichSide*piece),cellToPosition(row,col),whichSide,idCount);
    pieces[row][col].insert(toAdd);
    idCount++;
}

void boardCanvas::resetRects()
{
    for (int i=0; i<8; ++i){
        for (int j=0; j<8; ++j){
            sf::Vector2f toSet = cellToPosition(i,j);
            rectGrid[i][j].left = toSet.x;
            rectGrid[i][j].top = toSet.y;
        }
    }
}