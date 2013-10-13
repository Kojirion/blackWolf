#include "boardcanvas.h"
#include <boost/bimap/support/lambda.hpp>
#include <boost/assert.hpp>

const sf::Vector2f boardCanvas::offToCenter(25.f,25.f);

boardCanvas::boardCanvas(sf::Window& theWindow, resourceManager& theResources):
    flipOffset(0),
    window(sfg::Canvas::Create()),
    bigWindow(theWindow),
    resources(theResources),
    currentPiece(pieces.end()),
    idCount(1)
{
    boardSprite_.setTexture(resources.typeToTexture({Color::None, Piece::None}));

    piecesTexture.loadFromFile("Graphics/Pieces/ArrayBlack.png");

    particle.loadFromFile("Graphics/particle.png");
    system.reset(new thor::ParticleSystem());
    system->setTexture(particle);
    system->addAffector(FireworkAffector());

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

sf::Vector2i boardCanvas::toGridPos(const sf::Vector2f &position) const
{
    return sf::Vector2i((position.x-7*flipOffset-20)/(50-2*flipOffset),
                        (position.y+7*flipOffset-370)/(2*flipOffset-50)+1);
}

bool boardCanvas::pieceHeld()
{
    return currentPiece != pieces.end();
}

sf::Vector2f boardCanvas::cellToPosition(const Square &square) const
{
    return sf::Vector2f(flipOffset * (7 - 2*square.col) + 20 + 50 * square.col,
                        -flipOffset * (7 - 2*square.row) + 420 - 50 * (square.row+1));
}

void boardCanvas::releasePiece()
{
    currentPiece = pieces.end();
}

void boardCanvas::destroy(const Square& square)
{
    if (pieces.by<squareId>().erase(square)) //if true then firework
        system->addEmitter(FireworkEmitter(cellToPosition(square) + offToCenter), sf::seconds(1.f));
}

void boardCanvas::display()
{
    system->update(frameClock.restart());

    window->Clear();

    window->Draw(boardSprite_);
    window->Draw(*system, sf::BlendAdd);

    vertexArray.clear();
    for (const auto& piece : pieces)
        piece.get<pieceId>().appendQuadTo(vertexArray);

    window->Draw(vertexArray.data(), vertexArray.size(), sf::Quads, &piecesTexture);



    for (auto& arrow : arrows)
        window->Draw(arrow, sf::BlendAdd);


}

void boardCanvas::moveMake(const completeMove& move)
{
    const int originRow = move.getRow1();
    const int originCol = move.getCol1();
    const int destRow = move.getRow2();
    const int destCol = move.getCol2();

    destroy({destRow,destCol});
    bimapMove({{originRow, originCol}, {destRow, destCol}});
    pieces.by<squareId>().find(Square{destRow,destCol})->get<pieceId>().setPosition(cellToPosition({destRow,destCol}));
    //pieces[].moveTo(destRow, destCol, cellToPosition());

    position currentPosition = move.getNewBoard();
    if (currentPosition.wasCastle) handleCastle({destRow,destCol});
    if (currentPosition.wasEnPassant) handleEnPassant({destRow,destCol});
}

void boardCanvas::bimapMove(const Move &move)
{
    BOOST_VERIFY(pieces.by<squareId>().modify_key(pieces.by<squareId>().find(move.square_1), boost::bimaps::_key = move.square_2));
}

sfg::Widget::Ptr boardCanvas::getBoardWidget() const
{
    return window;
}

sf::Vector2f boardCanvas::getMousePosition() const
{
    return (static_cast<sf::Vector2f>(sf::Mouse::getPosition(bigWindow)) - window->GetAbsolutePosition());
}

void boardCanvas::sendBack()
{
    BOOST_ASSERT_MSG(pieceHeld(), "No current piece to send back");

    currentPiece->get<pieceId>().setPosition(cellToPosition(currentPiece->get<squareId>()));

    releasePiece();
}

void boardCanvas::flipBoard()
{
    if (flipped()) flipOffset = 0;
    else flipOffset = 50;

    for (const auto &piece : pieces)
        piece.get<pieceId>().setPosition(cellToPosition(piece.get<squareId>()));
}

void boardCanvas::reload(const position &givenPosition)
{
    pieces.clear();
    setPosition(givenPosition);
    boardSprite_.setTexture(resources.typeToTexture({Color::None, Piece::None}));
}

void boardCanvas::setResult(Color result)
{
    for (const auto& piece : pieces){
        if (!(piece.get<pieceId>().getColor() & result)){
            system->addEmitter(FireworkEmitter(piece.get<pieceId>().getPosition() + offToCenter), sf::seconds(1.f));
            //pieces.erase(piece);
        }
    }
}

void boardCanvas::setArrow(const Move &move)
{
    sf::Vector2f point1 = cellToPosition(move.square_1) + offToCenter;
    sf::Vector2f point2 = cellToPosition(move.square_2) + offToCenter;

    arrows.emplace_back(point1,point2-point1,sf::Color(0,100,0,125), 5.f);
}

void boardCanvas::clearArrows()
{
    arrows.clear();
}

void boardCanvas::handleCastle(const Square &square)
{
    if (square.row==0){
        if (square.col==2){
            bimapMove({{0,0},{0,3}});
            pieces.by<squareId>().find(Square{0,3})->get<pieceId>().setPosition(cellToPosition({0,3}));
        }else{
            BOOST_ASSERT_MSG(square.col==6, "Invalid Castle");
            bimapMove({{0,7},{0,5}});
            pieces.by<squareId>().find(Square{0,5})->get<pieceId>().setPosition(cellToPosition({0,5}));
        }
    }else{
        BOOST_ASSERT_MSG(square.row==7, "Invalid Castle");
        if (square.col==2){
            bimapMove({{7,0},{7,3}});
            pieces.by<squareId>().find(Square{7,3})->get<pieceId>().setPosition(cellToPosition({7,3}));
        }else{
            BOOST_ASSERT_MSG(square.col==6, "Invalid Castle");
            bimapMove({{7,7},{7,5}});
            pieces.by<squareId>().find(Square{7,5})->get<pieceId>().setPosition(cellToPosition({7,5}));
        }
    }
}

void boardCanvas::handleEnPassant(const Square &square)
{
    if (square.row==5){
        destroy({4,square.col});
    }else{
        BOOST_ASSERT_MSG(square.row==2, "Invalid en passant");
        destroy({3,square.col});
    }
}

void boardCanvas::slotLeftClick()
{
    //if (gameEnded) return; - it should be disabled from outside
    //if (humanColor != getTurnColor()) return; - boardMaster will decide that

    sf::Vector2f clickedPoint = getMousePosition();

    //const int whoseTurn = getTurnColor();

    for (const auto &piece : pieces){
        //if (piece.getSide()!=whoseTurn) continue;
        if (piece.get<pieceId>().contains(clickedPoint)){
            currentPiece = pieces.project_up(pieces.by<pieceId>().find(piece.get<pieceId>()));
            break;
        }
    }
}

void boardCanvas::slotMouseMove()
{
    if (pieceHeld()){
        currentPiece->get<pieceId>().setPosition(getMousePosition()-offToCenter);
    }
}

void boardCanvas::slotMouseRelease()
{
    if (pieceHeld()){
        sf::Vector2f centrePos = currentPiece->get<pieceId>().getPosition() + offToCenter;

        sf::Vector2i gridPos = toGridPos(centrePos);

        if (!(*requestMove({currentPiece->get<squareId>(), {gridPos.y, gridPos.x}})))
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
            const Unit pieceId = givenPosition(i, j);
            if ((pieceId.piece == Piece::None)||(pieceId.piece == Piece::Shadow)) continue;
            pieceSprite toAdd(cellToPosition({i,j}),pieceId, idCount++);
            pieces.insert(SquaresToPieces::value_type({i,j}, toAdd));
        }
    }

}

void boardCanvas::resetFor(Color whoFaceUp)
{
    if (flipped()){
        if (whoFaceUp & Color::White) flipBoard();
    }else{
        if (!(whoFaceUp & Color::White)) flipBoard();
    }    

    idCount = 1;
    pieces.clear();
}

boost::signals2::signal<bool (const Move&)>& boardCanvas::getSignal()
{
    return requestMove;
}

void boardCanvas::setPromotion(const Square &square, Piece piece)
{
    const Color whichSide = pieces.by<squareId>().find(square)->get<pieceId>().getColor();
    destroy(square);
    pieceSprite toAdd(cellToPosition(square),{whichSide, piece},idCount++);
    pieces.insert(SquaresToPieces::value_type(square, toAdd));
    //pieces[square].insert(toAdd);
}
