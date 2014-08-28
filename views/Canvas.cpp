#include "Canvas.hpp"
#include <boost/bimap/support/lambda.hpp>
#include <boost/assert.hpp>
#include <boost/cast.hpp>
#include "../controller/Controller.hpp"
//#include <boost/range/adaptor/filtered.hpp>

const sf::Vector2f Canvas::offToCenter(25.f,25.f);

Canvas::Canvas(sf::Window& theWindow):
    flipOffset(0),
    window(sfg::Canvas::Create()),
    bigWindow(theWindow),
    currentPiece(pieces.end()),
    idCount(1)
{
    boardTexture.loadFromFile("Graphics/BoardBlack.jpg");

    boardSprite_.setTexture(boardTexture);

    piecesTexture.loadFromFile("Graphics/Pieces/ArrayBlack.png");

    window->SetRequisition(sf::Vector2f( 440.f, 440.f ));
    window->GetSignal(sfg::Widget::OnMouseLeftPress).Connect(std::bind(&Canvas::slotLeftClick, this));
    window->GetSignal(sfg::Widget::OnMouseMove).Connect(std::bind(&Canvas::slotMouseMove, this));
    window->GetSignal(sfg::Widget::OnMouseLeftRelease).Connect(std::bind(&Canvas::slotMouseRelease, this));
    window->GetSignal(sfg::Widget::OnMouseEnter).Connect(std::bind(&Canvas::slotEnterCanvas, this));

    //    messages.connect("moveMade", [this](const Message& message){
    //        const MoveMessage* received = boost::polymorphic_downcast<const MoveMessage*>(&message);
    //        moveMake(received->move);
    //    });

    messages.connect("newGame", [this](const Message& message){
        const NewGameMessage* received = boost::polymorphic_downcast<const NewGameMessage*>(&message);
        resetFor(received->user);
    });

    messages.connect("endGame", [this](const Message& message){
        const EndGameMessage* received = boost::polymorphic_downcast<const EndGameMessage*>(&message);
        //setResult(received->result);
    });
}

bool Canvas::flipped() const
{
    return (flipOffset!=0);
}

Square Canvas::toGridPos(const sf::Vector2f &position) const
{
    if (!flipped())
        return {static_cast<int>(std::ceil((position.y+7*flipOffset-370)/(2*flipOffset-50))),
                    static_cast<int>(std::floor((position.x-7*flipOffset-20)/(50-2*flipOffset)))};
    else
        return {static_cast<int>(std::floor((position.y+7*flipOffset-370)/(2*flipOffset-50))),
                    static_cast<int>(std::ceil((position.x-7*flipOffset-20)/(50-2*flipOffset)))};
}

bool Canvas::pieceHeld()
{
    return currentPiece != pieces.end();
}

sf::Vector2f Canvas::cellToPosition(const Square &square) const
{
    return sf::Vector2f(flipOffset * (7 - 2*square.col) + 20 + 50 * square.col,
                        -flipOffset * (7 - 2*square.row) + 420 - 50 * (square.row+1));
}

void Canvas::releasePiece()
{
    currentPiece = pieces.end();
}

Color Canvas::getColorOn(const Square &square) const
{
    return pieces.by<squareId>().find(square)->get<pieceId>().getColor();
}

void Canvas::display()
{
    window->Clear();

    window->Draw(boardSprite_);

    vertexArray.clear();
    for (const auto& piece : pieces)
        piece.get<pieceId>().appendQuadTo(vertexArray);

    window->Draw(vertexArray.data(), vertexArray.size(), sf::Quads, &piecesTexture);



    for (auto& arrow : arrows)
        window->Draw(arrow, sf::BlendAdd);


}

void Canvas::moveMake(const std::vector<std::vector<Unit>>& position)
{
    pieces.clear();

    for (int i=0; i<8; ++i){
        for (int j=0; j<8; ++j){
            const Unit& unit = position[i][j];
            if (unit.piece == Piece::None) continue;
            PieceSprite toAdd(cellToPosition({7-i,j}),unit, idCount++);
            pieces.insert(SquaresToPieces::value_type({7-i,j}, toAdd));
        }
    }

//    const Move& toMake(move.getMove());

//    destroy(toMake.square_2);
//    bimapMove(toMake);
//    pieces.by<squareId>().find(toMake.square_2)->get<pieceId>().setPosition(cellToPosition(toMake.square_2));

//    Position currentPosition = move.getNewBoard();
//    if (currentPosition.wasCastle) handleCastle(toMake.square_2);
//    if (currentPosition.wasEnPassant) handleEnPassant(toMake.square_2);
}

sfg::Widget::Ptr Canvas::getBoardWidget() const
{
    return window;
}

sf::Vector2f Canvas::getMousePosition() const
{
    return (static_cast<sf::Vector2f>(sf::Mouse::getPosition(bigWindow)) - window->GetAbsolutePosition());
}

void Canvas::sendBack()
{
    BOOST_ASSERT_MSG(pieceHeld(), "No current piece to send back");

    currentPiece->get<pieceId>().setPosition(cellToPosition(currentPiece->get<squareId>()));

    releasePiece();
}

void Canvas::flipBoard()
{
    if (flipped()) flipOffset = 0;
    else flipOffset = 50;

    for (const auto &piece : pieces)
        piece.get<pieceId>().setPosition(cellToPosition(piece.get<squareId>()));
}

void Canvas::setArrow(const Move &move)
{
    sf::Vector2f point1 = cellToPosition(move.square_1) + offToCenter;
    sf::Vector2f point2 = cellToPosition(move.square_2) + offToCenter;

    arrows.emplace_back(point1,point2-point1,sf::Color(0,100,0,125), 5.f);
}

void Canvas::clearArrows()
{
    arrows.clear();
}

void Canvas::slotLeftClick()
{
    //if (gameEnded) return; - it should be disabled from outside
    //if (humanColor != getTurnColor()) return; - boardMaster will decide that

    sf::Vector2f clickedPoint = getMousePosition();

    //const int whoseTurn = getTurnColor();

    for (const auto &piece : pieces){
        //if (piece.getSide()!=whoseTurn) continue;
        if (piece.get<pieceId>().contains(clickedPoint)){
            currentPiece = pieces.project_up(pieces.by<pieceId>().find(piece.get<pieceId>()));
            pieceOffset = currentPiece->get<pieceId>().getPosition() - clickedPoint;
            break;
        }
    }
}

void Canvas::slotMouseMove()
{
    if (pieceHeld()){
        currentPiece->get<pieceId>().setPosition(getMousePosition() + pieceOffset);
    }
}

void Canvas::slotMouseRelease()
{
    if (pieceHeld()){
        sf::Vector2f centrePos = currentPiece->get<pieceId>().getPosition() + offToCenter;

        Square gridPos = toGridPos(centrePos);

        requestMove({currentPiece->get<squareId>(), gridPos});
        releasePiece();
    }
}

void Canvas::slotEnterCanvas()
{
    if (pieceHeld()){
        if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) sendBack();
    }
}

void Canvas::resetFor(Color whoFaceUp)
{
    if (flipped()){
        if (whoFaceUp & Color::White) flipBoard();
    }else{
        if (!(whoFaceUp & Color::White)) flipBoard();
    }

    idCount = 1;
    pieces.clear();
}

boost::signals2::signal<bool (const Move&)>& Canvas::getSignal()
{
    return requestMove;
}
