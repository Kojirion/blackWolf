#include "boardmaster.h"

boardMaster::boardMaster(sf::Window &theWindow):
    flipOffset(0,0),
    window_(sfg::Canvas::Create()),
    currentPiece(nullptr),
    bigWindow(theWindow),
    turnColor(1)
{
    window_->SetRequisition(sf::Vector2f( 440.f, 440.f ));
    window_->GetSignal(sfg::Widget::OnMouseLeftPress).Connect(&boardMaster::processLeftClick, this);
    window_->GetSignal(sfg::Widget::OnMouseMove).Connect(&boardMaster::processMouseMove, this);
    window_->GetSignal(sfg::Widget::OnMouseLeftRelease).Connect(&boardMaster::processMouseRelease, this);


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

    pieces.emplace_back(blackRookT,cellToPosition(7,0),-1);
    pieces.emplace_back(blackRookT,cellToPosition(7,7),-1);
    pieces.emplace_back(blackKnightT,cellToPosition(7,1),-1);
    pieces.emplace_back(blackKnightT,cellToPosition(7,6),-1);
    pieces.emplace_back(blackBishopT,cellToPosition(7,2),-1);
    pieces.emplace_back(blackBishopT,cellToPosition(7,5),-1);
    pieces.emplace_back(blackQueenT,cellToPosition(7,3),-1);
    pieces.emplace_back(blackKingT,cellToPosition(7,4),-1);
    for (int i=0; i<8; ++i)
        pieces.emplace_back(blackPawnT,cellToPosition(6,i),-1);
    pieces.emplace_back(whiteRookT,cellToPosition(0,0),1);
    pieces.emplace_back(whiteRookT,cellToPosition(0,7),1);
    pieces.emplace_back(whiteKnightT,cellToPosition(0,1),1);
    pieces.emplace_back(whiteKnightT,cellToPosition(0,6),1);
    pieces.emplace_back(whiteBishopT,cellToPosition(0,2),1);
    pieces.emplace_back(whiteBishopT,cellToPosition(0,5),1);
    pieces.emplace_back(whiteQueenT,cellToPosition(0,3),1);
    pieces.emplace_back(whiteKingT,cellToPosition(0,4),1);
    for (int i=0; i<8; ++i)
        pieces.emplace_back(whitePawnT,cellToPosition(1,i),1);

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




}

void boardMaster::display()
{
    window_->Clear();

    window_->Draw(boardSprite_);

    for (auto piece : pieces) window_->Draw(piece);

    //window_->Display();

}

sf::Vector2f boardMaster::cellToPosition(const int row, const int col)
{
    return sf::Vector2f(flipOffset.x * (9 - 2 * col) + 20 + 50 * col, (flipOffset.y * (9 - 2 * row)) + 420 - 50 * (row+1));
}

sf::Vector2f boardMaster::getMousePosition()
{
    sf::Vector2f windowPos = window_->GetAbsolutePosition() + static_cast<sf::Vector2f>(bigWindow.getPosition());
    return (static_cast<sf::Vector2f>(sf::Mouse::getPosition()) - windowPos);
}

int boardMaster::getTurnColor() const
{
    return turnColor;
}

void boardMaster::processLeftClick()
{
    clickedPoint = getMousePosition();

    //std::cout << clickedPoint.x << " " << clickedPoint.y << std::endl;

    for (auto &piece : pieces){
        if (piece.contains(clickedPoint)){
            if (piece.getSide()!=turnColor) return;
            currentPiece = &piece;
            break;
        }
    }
}

void boardMaster::processMouseMove()
{
    if (currentPiece){
        /*sf::Vector2f mouseCurrentPoint = getMousePosition();
        sf::Vector2f mouseDifference = (mouseCurrentPoint - clickedPoint);
        clickedPoint = mouseCurrentPoint;*/

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
                    currentPiece->setPosition(rectGrid[i][j].left,rectGrid[i][j].top);
                    currentPiece = nullptr;
                    turnColor *= -1;
                    return;
                }
            }
        }
        currentPiece = nullptr;
    }
}
