#include "Canvas.hpp"
#include <boost/bimap/support/lambda.hpp>
#include <cassert>
#include <boost/cast.hpp>
#include "../controller/Controller.hpp"
#include "components/Emitter.hpp"
#include <boost/range/irange.hpp>
#include <boost/range/algorithm/find_if.hpp>
#include "../messages/GameState.hpp"
#include "../messages/GameStart.hpp"

class Grid {
public:
    Grid(int squareSide, int margin):
        squareSide(squareSide),
        margin(margin),
        flipped(false)
    {

    }

    sf::Vector2f centerOffset() const{
        return {squareSide/2.f, squareSide/2.f};
    }

    Square toSquare(const sf::Vector2f& coords) const{
        if (flipped)
            return {static_cast<int>(std::floor((coords.y+7*squareSide-7*squareSide-margin)/(2*squareSide-squareSide))),
                        static_cast<int>(std::ceil((coords.x-7*squareSide-margin)/(squareSide-2*squareSide)))};
        else
            return {static_cast<int>(std::ceil((coords.y-7*squareSide-margin)/(-squareSide))),
                        static_cast<int>(std::floor((coords.x-margin)/squareSide))};
    }

    sf::Vector2f toCoords(const Square& square) const {
        sf::Vector2f result{margin + squareSide*square.col,
                    8*squareSide + margin - squareSide*(square.row+1)};

        if (flipped)
            result += {squareSide* (7 - 2*square.col), -squareSide * (7 - 2*square.row)};

        return result;
    }

    float boardSide() const {
        return 8*squareSide + 2*margin;
    }

    void flip(){
        flipped = !flipped;
    }

    bool isFlipped() const {
        return flipped;
    }

private:
    float squareSide;
    float margin;
    bool flipped;
};

static Grid grid{50, 20};


using boost::irange;

Canvas::Canvas(sf::Window& theWindow):
    m_currentPiece(m_pieces.end()),
    m_canvas(sfg::Canvas::Create()),
    m_applicationWindow(theWindow),
    m_previousTurnColor(Color::Black),
    m_idCount(1),
    m_pieceToTexPos{1,0}
{
    m_boardTexture.loadFromFile("Graphics/BoardBlack.jpg");

    m_boardSprite.setTexture(m_boardTexture);

    m_piecesTexture.loadFromFile("Graphics/Pieces.png");

    //prepare fragments for capture animations
    m_particleSystem.setTexture(m_piecesTexture);
    m_particleSystem.addAffector(thor::TorqueAffector(100.f));

    int l = 50/nr_fragments_side;
    for (auto i : irange(0,12)){
        auto pieceTexPos = static_cast<sf::Vector2i>(m_pieceToTexPos(IntToPiece[i]));
        for (auto j : irange(0, nr_fragments_side)){
            for (auto k : irange(0,nr_fragments_side)){
                auto fragmentTexPos = pieceTexPos + sf::Vector2i(l*j, k*l);
                sf::IntRect rect(fragmentTexPos, sf::Vector2i(l, l));
                m_particleSystem.addTextureRect(rect);
            }
        }
    }

    m_canvas->SetRequisition(sf::Vector2f( 440.f, 440.f ));

    m_canvas->GetSignal(sfg::Widget::OnMouseLeftPress).Connect([this]{
        auto clickedPoint = getMousePosition();

        for (const auto &piece : m_pieces){
            if (piece.get<Sprite>().contains(clickedPoint)){
                m_currentPiece = m_pieces.project_up(m_pieces.by<Sprite>().find(piece.get<Sprite>()));
                m_pieceOffset = m_currentPiece->get<Sprite>().getPosition() - clickedPoint;
                break;
            }
        }
    });

    m_canvas->GetSignal(sfg::Widget::OnMouseMove).Connect([this]{
        if (pieceHeld())
            m_currentPiece->get<Sprite>().setPosition(getMousePosition() + m_pieceOffset);
    });

    m_canvas->GetSignal(sfg::Widget::OnMouseLeftRelease).Connect([this]{
        if (pieceHeld()){
            auto centrePos = m_currentPiece->get<Sprite>().getPosition() + grid.centerOffset();
            auto gridPos = grid.toSquare(centrePos);

            if (!(*requestMove({m_currentPiece->get<OriginSquare>(), gridPos})))
                sendBack();
            else {
                m_currentPiece->get<Sprite>().setPosition(grid.toCoords(gridPos));
                releasePiece();
            }
        }
    });

    m_canvas->GetSignal(sfg::Widget::OnMouseEnter).Connect([this]{
        if (pieceHeld()){
            if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
                sendBack();
        }
    });

    messages.connect(Messages::ID::GameStart, [this](const Messages::Message& message){
        auto received = boost::polymorphic_downcast<const Messages::GameStart*>(&message);
        resetFor(received->user);
    });

    messages.connect(Messages::ID::GameState, [this](const Messages::Message& message){
        auto received = boost::polymorphic_downcast<const Messages::GameState*>(&message);
        if (m_previousTurnColor != received->turnColor){
            m_previousTurnColor = received->turnColor;
            if (received->target_square)
                animateCaptureOn(*received->target_square);
        }
        setupBoard(received->position, received->turnColor);
    });
}

bool Canvas::pieceHeld() const
{
    return m_currentPiece != m_pieces.end();
}

void Canvas::releasePiece()
{
    m_currentPiece = m_pieces.end();
}

void Canvas::setPieceColors(const PieceToTexPos& pieceToTexPos)
{
    m_pieceToTexPos = pieceToTexPos;
    for (const auto& piece : m_pieces)
        piece.get<Sprite>().setTexPos(m_pieceToTexPos);
}

Color Canvas::getColorOn(const Square &square) const
{
    return m_pieces.by<OriginSquare>().find(square)->get<Sprite>().getColor();
}

void Canvas::update(sf::Time dt)
{
    m_particleSystem.update(dt);
    m_canvas->Clear();

    m_canvas->Draw(m_boardSprite);
    m_canvas->Draw(m_particleSystem);

    m_pieceVertices.clear();
    for (const auto& piece : m_pieces)
        piece.get<Sprite>().appendQuadTo(m_pieceVertices);

    m_canvas->Draw(m_pieceVertices.data(), m_pieceVertices.size(), sf::Quads, &m_piecesTexture);



    for (auto& arrow : m_arrows)
        m_canvas->Draw(arrow, sf::BlendAdd);


}

void Canvas::setupBoard(const std::vector<std::vector<Piece>>& position, Color turnColor)
{
    Square square;
    sf::Vector2f pos;
    auto wasPieceHeld = pieceHeld();

    if (wasPieceHeld){
        square = m_currentPiece->get<OriginSquare>();
        pos = m_currentPiece->get<Sprite>().getPosition();
        releasePiece();
    }

    m_pieces.clear();

    for (auto i : irange(0,8)){
        for (auto j : irange(0,8)){
            const Piece& piece = position[i][j];
            if (piece.type == Piece::Type::None) continue;
            PieceSprite toAdd(grid.toCoords({7-i,j}), piece, m_pieceToTexPos, m_idCount++);
            m_pieces.insert(SquaresToPieces::value_type({7-i,j}, toAdd));
        }
    }

    if (wasPieceHeld){
        auto it = m_pieces.by<OriginSquare>().find(square);
        if (it->get<Sprite>().getColor() == turnColor){
            m_currentPiece = m_pieces.project_up(it);
            m_currentPiece->get<Sprite>().setPosition(pos);
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

    m_currentPiece->get<Sprite>().setPosition(grid.toCoords(m_currentPiece->get<OriginSquare>()));

    releasePiece();
}

void Canvas::flipBoard()
{
    grid.flip();

    for (const auto &piece : m_pieces)
        piece.get<Sprite>().setPosition(grid.toCoords(piece.get<OriginSquare>()));
}

void Canvas::setPremove(const Move &move)
{
    auto from = grid.toCoords(move.square_1) + grid.centerOffset();
    auto to = grid.toCoords(move.square_2) + grid.centerOffset();

    m_arrows.emplace_back(from, to-from, sf::Color(0,100,0,125), 5.f);
}

void Canvas::clearArrows()
{
    m_arrows.clear();
}

void Canvas::resetFor(Color whoFaceUp)
{
    if (grid.isFlipped()){
        if (whoFaceUp & Color::White)
            grid.flip();
    }else{
        if (!(whoFaceUp & Color::White))
            grid.flip();
    }

    m_idCount = 1;
    m_pieces.clear();
}

void Canvas::animateCaptureOn(const Square& square)
{
    auto it = m_pieces.by<OriginSquare>().find(square);
    if (it != m_pieces.by<OriginSquare>().end()){
        auto piece = it->second.getPiece();
        m_particleSystem.addEmitter(Emitter(grid.toCoords(square) + grid.centerOffset(), piece), sf::seconds(0.000001f));
    }
}
