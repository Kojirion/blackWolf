#include "Emitter.hpp"
#include <Thor/Particles/EmissionInterface.hpp>
#include <Thor/Particles/Particle.hpp>
#include <boost/range/irange.hpp>

using boost::irange;

const int nr_fragments_side = 2;
const int nr_fragments = nr_fragments_side*nr_fragments_side;
const Piece IntToPiece[12]  = {Piece{Color::White, Piece::Type::Pawn},
                         {Color::White, Piece::Type::Rook},
                         {Color::White, Piece::Type::Knight},
                         {Color::White, Piece::Type::Bishop},
                         {Color::White, Piece::Type::King},
                         {Color::White, Piece::Type::Queen},
                         {Color::Black, Piece::Type::Pawn},
                         {Color::Black, Piece::Type::Rook},
                         {Color::Black, Piece::Type::Knight},
                         {Color::Black, Piece::Type::Bishop},
                         {Color::Black, Piece::Type::King},
                         {Color::Black, Piece::Type::Queen}
                        };

int PieceToInt(const Piece &piece)
{
    for (auto i : irange(0, 12)){
        if (IntToPiece[i]==piece)
            return i*nr_fragments;
    }
    return 0;
}

Emitter::Emitter(const sf::Vector2f &position, const Piece &piece):
    m_position(position), m_index(PieceToInt(piece))
{

}

void Emitter::operator()(thor::EmissionInterface &system, sf::Time dt)
{
    thor::Particle particle(sf::seconds(1.5f));
    particle.position = m_position;
    particle.textureIndex = m_index;

    for (auto i : irange(0, nr_fragments)){
        using thor::random;

        thor::PolarVector2f velocity(random(100.f, 200.f), random(0.f, 360.f));
        particle.velocity = velocity;
        system.emitParticle(particle);
    }
}



