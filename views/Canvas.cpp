#include "Canvas.hpp"
#include <boost/bimap/support/lambda.hpp>
#include <boost/assert.hpp>
#include <boost/cast.hpp>
#include "../controller/Controller.hpp"
#include "components/Emitter.hpp"

//#include <boost/range/adaptor/filtered.hpp>

const sf::Vector2f Canvas::offToCenter(25.f,25.f);

Canvas::Canvas(sf::Window& theWindow):
    flipOffset(0),
    window(sfg::Canvas::Create()),
    bigWindow(theWindow),
    currentPiece(pieces.end()),
    idCount(1),
    previousTurnColor(Color::Black)
{
    boardTexture.loadFromFile("Graphics/BoardBlack.jpg");

    boardSprite_.setTexture(boardTexture);

    piecesTexture.loadFromFile("Graphics/Pieces/ArrayBlack.png");

    //prepare fragments for capture animations
    m_particleSystem.setTexture(piecesTexture);
    m_particleSystem.addAffector( thor::TorqueAffector(100.f) );

    int l = 50/nr_fragments_side;
    for (int i=0; i<12; ++i){
        auto unitTexPos = static_cast<sf::Vector2i>(typeToTexPos(IntToUnit[i]));
        for (int j=0; j<nr_fragments_side; ++j){
            for (int k=0; k<nr_fragments_side; ++k){
                auto fragmentTexPos = unitTexPos + sf::Vector2i(l*j, k*l);
                sf::IntRect rect(fragmentTexPos, sf::Vector2i(l, l));
                m_particleSystem.addTextureRect(rect);
            }
        }
    }

    //connect signals and messages to slots
    window->SetRequisition(sf::Vector2f( 440.f, 440.f ));
    window->GetSignal(sfg::Widget::OnMouseLeftPress).Connect(std::bind(&Canvas::slotLeftClick, this));
    window->GetSignal(sfg::Widget::OnMouseMove).Connect(std::bind(&Canvas::slotMouseMove, this));
    window->GetSignal(sfg::Widget::OnMouseLeftRelease).Connect(std::bind(&Canvas::slotMouseRelease, this));
    window->GetSignal(sfg::Widget::OnMouseEnter).Connect(std::bind(&Canvas::slotEnterCanvas, this));

    messages.connect("newGame", [this](const Message& message){
        auto received = boost::polymorphic_downcast<const NewGameMessage*>(&message);
        resetFor(received->user);
    });

    messages.connect("gameState", [this](const Message& message){
        auto received = boost::polymorphic_downcast<const GameStateMessage*>(&message);
        if (previousTurnColor != received->turnColor){
            previousTurnColor = received->turnColor;
            if (received->target_square)
                destroy(*received->target_square);
        }
        setupBoard(received->position, received->turnColor);
    });
}

bool Canvas::flipped() const
{
    return (flipOffset!=0);
}

Square Canvas::positionToSquare(const sf::Vector2f &position) const
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

sf::Vector2f Canvas::squareToPosition(const Square &square) const
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
    m_particleSystem.update(frameClock.restart());
    window->Clear();

    window->Draw(boardSprite_);
    window->Draw(m_particleSystem);

    vertexArray.clear();
    for (const auto& piece : pieces)
        piece.get<pieceId>().appendQuadTo(vertexArray);

    window->Draw(vertexArray.data(), vertexArray.size(), sf::Quads, &piecesTexture);



    for (auto& arrow : arrows)
        window->Draw(arrow, sf::BlendAdd);


}

void Canvas::setupBoard(const std::vector<std::vector<Unit>>& position, Color turnColor)
{
    Square square;
    sf::Vector2f pos;
    auto wasPieceHeld = pieceHeld();

    if (wasPieceHeld){
        square = currentPiece->get<squareId>();
        pos = currentPiece->get<pieceId>().getPosition();
        releasePiece();
    }

    pieces.clear();

    for (int i=0; i<8; ++i){
        for (int j=0; j<8; ++j){
            const Unit& unit = position[i][j];
            if (unit.piece == Piece::None) continue;
            PieceSprite toAdd(squareToPosition({7-i,j}),unit, idCount++);
            pieces.insert(SquaresToPieces::value_type({7-i,j}, toAdd));
        }
    }

    if (wasPieceHeld){
        auto it = pieces.by<squareId>().find(square);
        if (it->get<pieceId>().getColor() == turnColor){
            currentPiece = pieces.project_up(it);
            currentPiece->get<pieceId>().setPosition(pos);
        }
    }


    //    const Move& toMake(move.getMove());

    //    destroy(toMake.square_2);
    //bimapMove(toMake);
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

    currentPiece->get<pieceId>().setPosition(squareToPosition(currentPiece->get<squareId>()));

    releasePiece();
}

void Canvas::flipBoard()
{
    if (flipped()) flipOffset = 0;
    else flipOffset = 50;

    for (const auto &piece : pieces)
        piece.get<pieceId>().setPosition(squareToPosition(piece.get<squareId>()));
}

void Canvas::setArrow(const Move &move)
{
    sf::Vector2f point1 = squareToPosition(move.square_1) + offToCenter;
    sf::Vector2f point2 = squareToPosition(move.square_2) + offToCenter;

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

        Square gridPos = positionToSquare(centrePos);

        if (!(*requestMove({currentPiece->get<squareId>(), gridPos})))
            sendBack();
        else {
            currentPiece->get<pieceId>().setPosition(squareToPosition(gridPos));
            releasePiece();
        }
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

void Canvas::destroy(const Square& square)
{
    auto it = pieces.by<squareId>().find(square);
    if (it != pieces.by<squareId>().end()){
        auto unit = it->second.getUnit();
        m_particleSystem.addEmitter(Emitter(squareToPosition(square) + offToCenter, unit), sf::seconds(0.000001f));
    }
}
