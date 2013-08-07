#include "boardmaster.h"
#include <sstream>
#include <boost/assert.hpp>

void boardMaster::destroy(const int row, const int col)
{
    std::vector<pieceSprite>::iterator it;
    std::vector<pieceSprite>::const_iterator itEnd = pieces.end();

    for (it = pieces.begin(); it!=itEnd; ++it){
        if ((it->row==row)&&(it->col==col)){
            pieces.erase(it);
            return;
        }
    }

}

boardMaster::boardMaster(sf::Window &theWindow):
    flipOffset(0,0),
    window_(sfg::Canvas::Create()),
    currentPiece(nullptr),
    bigWindow(theWindow),
    turnLabel_(sfg::Label::Create("White to play")),
    whiteClockCanvas_(sfg::Canvas::Create()),
    blackClockCanvas_(sfg::Canvas::Create()),
    moveList(sfg::Table::Create()),
    plyCounter(0)
{
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
            //const sf::Texture &pieceTexture = idToTexture(pieceId);
            pieces.emplace_back(idToTexture(pieceId),cellToPosition(i,j),pieceId,i,j);
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

void boardMaster::display()
{
    window_->Clear();

    window_->Draw(boardSprite_);

    for (auto piece : pieces) window_->Draw(piece);

    updateClocks();

    //window_->Display();

}

sf::Vector2f boardMaster::cellToPosition(const int row, const int col) const
{
    return sf::Vector2f(flipOffset.x * (9 - 2 * col) + 20 + 50 * col, (flipOffset.y * (9 - 2 * row)) + 420 - 50 * (row+1));
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
    sf::Vector2f windowPos = window_->GetAbsolutePosition() + static_cast<sf::Vector2f>(bigWindow.getPosition());
    return (static_cast<sf::Vector2f>(sf::Mouse::getPosition()) - windowPos);
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
}

void boardMaster::sendBack()
{
    BOOST_ASSERT_MSG(currentPiece, "No current piece to send back");

    currentPiece->setPosition(cellToPosition(currentPiece->row,currentPiece->col));
    currentPiece = nullptr;
}

void boardMaster::processLeftClick()
{
    clickedPoint = getMousePosition();

    const int whoseTurn = getTurnColor();

    for (auto &piece : pieces){
        if (piece.contains(clickedPoint)){
            if (piece.getSide()!=whoseTurn) return;
            currentPiece = &piece;
            break;
        }
    }
}

void boardMaster::processMouseMove()
{
    if (currentPiece){        
        currentPiece->setPosition(getMousePosition()-sf::Vector2f(25.f,25.f));
    }
}

void boardMaster::processMouseRelease()
{
    if (currentPiece){
        sf::Vector2f centrePos = currentPiece->getPosition() + sf::Vector2f(25.f,25.f);
        for (int i=0; i<8; ++i){
            for (int j=0; j<8; ++j){
                if (rectGrid[i][j].contains(centrePos)){
                    const int originRow = currentPiece->row;
                    const int originCol = currentPiece->col;
                    completeMove toCheck(currentPosition,originRow,originCol,i,j);
                    if (toCheck.isLegal()){
                        destroy(i,j);
                        currentPiece->setPosition(rectGrid[i][j].left,rectGrid[i][j].top);
                        currentPiece->setCell(i,j);
                        currentPiece = nullptr;
                        currentPosition = toCheck.getNewBoard();
                        switchTurn();
                        sfg::Label::Ptr newMove(sfg::Label::Create(moveToString(originRow,originCol,i,j)));
                        const int plyPairsCount = plyCounter/2;
                        const int plyRemainder = (plyCounter)%2;
                        moveList->Attach(newMove,{plyRemainder,plyPairsCount,1,1});
                        plyCounter++;
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
    if (currentPiece){
        if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) sendBack();
    }
}

std::string boardMaster::toString(sf::Time value) const
{
    int minutes = value.asSeconds()/60;
    int seconds = static_cast<int>(value.asSeconds())%60;

    std::ostringstream stream;
    stream.setf(std::ios_base::fixed);
    stream.precision(2);
    stream << minutes << ":" << seconds;
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
    whiteClockText.setString(toString(whiteClock.getRemainingTime()));
    whiteClockCanvas_->Clear();
    whiteClockCanvas_->Draw(whiteClockText);

    blackClockText.setString(toString(blackClock.getRemainingTime()));
    blackClockCanvas_->Clear();
    blackClockCanvas_->Draw(blackClockText);    
}
