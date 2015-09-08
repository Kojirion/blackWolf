#include "Canvas.hpp"
#include <boost/bimap/support/lambda.hpp>
#include <cassert>
#include <boost/cast.hpp>
#include "../controller/Controller.hpp"
#include "components/Emitter.hpp"

const sf::Vector2f Canvas::m_offToCenter(25.f,25.f);

Canvas::Canvas(sf::Window& theWindow):
    m_currentPiece(m_pieces.end()),
    m_flipOffset(0),
    m_canvas(sfg::Canvas::Create()),
    m_applicationWindow(theWindow),
    m_previousTurnColor(Color::Black),
    m_idCount(1)
{
    m_boardTexture.loadFromFile("Graphics/BoardBlack.jpg");

    m_boardSprite.setTexture(m_boardTexture);

    m_piecesTexture.loadFromFile("Graphics/Pieces/ArrayBlack.png");

    //prepare fragments for capture animations
    m_particleSystem.setTexture(m_piecesTexture);
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
    m_canvas->SetRequisition(sf::Vector2f( 440.f, 440.f ));
    m_canvas->GetSignal(sfg::Widget::OnMouseLeftPress).Connect(std::bind(&Canvas::slotLeftClick, this));
    m_canvas->GetSignal(sfg::Widget::OnMouseMove).Connect(std::bind(&Canvas::slotMouseMove, this));
    m_canvas->GetSignal(sfg::Widget::OnMouseLeftRelease).Connect(std::bind(&Canvas::slotMouseRelease, this));
    m_canvas->GetSignal(sfg::Widget::OnMouseEnter).Connect(std::bind(&Canvas::slotEnterCanvas, this));

    messages.connect("newGame", [this](const Message& message){
        auto received = boost::polymorphic_downcast<const NewGameMessage*>(&message);
        resetFor(received->user);
    });

    messages.connect("gameState", [this](const Message& message){
        auto received = boost::polymorphic_downcast<const GameStateMessage*>(&message);
        if (m_previousTurnColor != received->turnColor){
            m_previousTurnColor = received->turnColor;
            if (received->target_square)
                animateCaptureOn(*received->target_square);
        }
        setupBoard(received->position, received->turnColor);
    });
}

bool Canvas::flipped() const
{
    return (m_flipOffset!=0);
}

Square Canvas::positionToSquare(const sf::Vector2f &position) const
{
    if (!flipped())
        return {static_cast<int>(std::ceil((position.y+7*m_flipOffset-370)/(2*m_flipOffset-50))),
                    static_cast<int>(std::floor((position.x-7*m_flipOffset-20)/(50-2*m_flipOffset)))};
    else
        return {static_cast<int>(std::floor((position.y+7*m_flipOffset-370)/(2*m_flipOffset-50))),
                    static_cast<int>(std::ceil((position.x-7*m_flipOffset-20)/(50-2*m_flipOffset)))};
}

bool Canvas::pieceHeld() const
{
    return m_currentPiece != m_pieces.end();
}

sf::Vector2f Canvas::squareToPosition(const Square &square) const
{
    return sf::Vector2f(m_flipOffset * (7 - 2*square.col) + 20 + 50 * square.col,
                        -m_flipOffset * (7 - 2*square.row) + 420 - 50 * (square.row+1));
}

void Canvas::releasePiece()
{
    m_currentPiece = m_pieces.end();
}

Color Canvas::getColorOn(const Square &square) const
{
    return m_pieces.by<squareId>().find(square)->get<pieceId>().getColor();
}

void Canvas::display()
{
    m_particleSystem.update(m_frameClock.restart());
    m_canvas->Clear();

    m_canvas->Draw(m_boardSprite);
    m_canvas->Draw(m_particleSystem);

    m_pieceVertices.clear();
    for (const auto& piece : m_pieces)
        piece.get<pieceId>().appendQuadTo(m_pieceVertices);

    m_canvas->Draw(m_pieceVertices.data(), m_pieceVertices.size(), sf::Quads, &m_piecesTexture);



    for (auto& arrow : m_arrows)
        m_canvas->Draw(arrow, sf::BlendAdd);


}

void Canvas::setupBoard(const std::vector<std::vector<Unit>>& position, Color turnColor)
{
    Square square;
    sf::Vector2f pos;
    auto wasPieceHeld = pieceHeld();

    if (wasPieceHeld){
        square = m_currentPiece->get<squareId>();
        pos = m_currentPiece->get<pieceId>().getPosition();
        releasePiece();
    }

    m_pieces.clear();

    for (int i=0; i<8; ++i){
        for (int j=0; j<8; ++j){
            const Unit& unit = position[i][j];
            if (unit.piece == Piece::None) continue;
            PieceSprite toAdd(squareToPosition({7-i,j}),unit, m_idCount++);
            m_pieces.insert(SquaresToPieces::value_type({7-i,j}, toAdd));
        }
    }

    if (wasPieceHeld){
        auto it = m_pieces.by<squareId>().find(square);
        if (it->get<pieceId>().getColor() == turnColor){
            m_currentPiece = m_pieces.project_up(it);
            m_currentPiece->get<pieceId>().setPosition(pos);
        }
    }
}

sfg::Widget::Ptr Canvas::getBoardWidget() const
{
    return m_canvas;
}

sf::Vector2f Canvas::getMousePosition() const
{
    return (static_cast<sf::Vector2f>(sf::Mouse::getPosition(m_applicationWindow)) - m_canvas->GetAbsolutePosition());
}

void Canvas::sendBack()
{
    assert(pieceHeld());

    m_currentPiece->get<pieceId>().setPosition(squareToPosition(m_currentPiece->get<squareId>()));

    releasePiece();
}

void Canvas::flipBoard()
{
    if (flipped()) m_flipOffset = 0;
    else m_flipOffset = 50;

    for (const auto &piece : m_pieces)
        piece.get<pieceId>().setPosition(squareToPosition(piece.get<squareId>()));
}

void Canvas::setPremove(const Move &move)
{
    sf::Vector2f point1 = squareToPosition(move.square_1) + m_offToCenter;
    sf::Vector2f point2 = squareToPosition(move.square_2) + m_offToCenter;

    m_arrows.emplace_back(point1,point2-point1,sf::Color(0,100,0,125), 5.f);
}

void Canvas::clearArrows()
{
    m_arrows.clear();
}

void Canvas::slotLeftClick()
{
    sf::Vector2f clickedPoint = getMousePosition();

    for (const auto &piece : m_pieces){
        if (piece.get<pieceId>().contains(clickedPoint)){
            m_currentPiece = m_pieces.project_up(m_pieces.by<pieceId>().find(piece.get<pieceId>()));
            m_pieceOffset = m_currentPiece->get<pieceId>().getPosition() - clickedPoint;
            break;
        }
    }
}

void Canvas::slotMouseMove()
{
    if (pieceHeld())
        m_currentPiece->get<pieceId>().setPosition(getMousePosition() + m_pieceOffset);
}

void Canvas::slotMouseRelease()
{
    if (pieceHeld()){
        sf::Vector2f centrePos = m_currentPiece->get<pieceId>().getPosition() + m_offToCenter;

        Square gridPos = positionToSquare(centrePos);

        if (!(*requestMove({m_currentPiece->get<squareId>(), gridPos})))
            sendBack();
        else {
            m_currentPiece->get<pieceId>().setPosition(squareToPosition(gridPos));
            releasePiece();
        }
    }
}

void Canvas::slotEnterCanvas()
{
    if (pieceHeld()){
        if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
            sendBack();
    }
}

void Canvas::resetFor(Color whoFaceUp)
{
    if (flipped()){
        if (whoFaceUp & Color::White)
            flipBoard();
    }else{
        if (!(whoFaceUp & Color::White))
            flipBoard();
    }

    m_idCount = 1;
    m_pieces.clear();
}

void Canvas::animateCaptureOn(const Square& square)
{
    auto it = m_pieces.by<squareId>().find(square);
    if (it != m_pieces.by<squareId>().end()){
        auto unit = it->second.getUnit();
        m_particleSystem.addEmitter(Emitter(squareToPosition(square) + m_offToCenter, unit), sf::seconds(0.000001f));
    }
}
