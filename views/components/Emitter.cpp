#include "Emitter.hpp"
#include <Thor/Particles/EmissionInterface.hpp>
#include <Thor/Particles/Particle.hpp>

const int nr_fragments_side = 2;
const int nr_fragments = nr_fragments_side*nr_fragments_side;
const Unit IntToUnit[12]  = {Unit{Color::White, Piece::Pawn},
                         {Color::White, Piece::Rook},
                         {Color::White, Piece::Knight},
                         {Color::White, Piece::Bishop},
                         {Color::White, Piece::King},
                         {Color::White, Piece::Queen},
                         {Color::Black, Piece::Pawn},
                         {Color::Black, Piece::Rook},
                         {Color::Black, Piece::Knight},
                         {Color::Black, Piece::Bishop},
                         {Color::Black, Piece::King},
                         {Color::Black, Piece::Queen}
                        };

int UnitToInt(const Unit &unit)
{
    for (int i=0; i<12; ++i){
        if (IntToUnit[i]==unit)
            return i*nr_fragments;
    }
    return 0;
}

Emitter::Emitter(const sf::Vector2f &position, const Unit &unit):
    m_position(position), m_index(UnitToInt(unit))
{

}

void Emitter::operator()(thor::EmissionInterface &system, sf::Time dt)
{
    thor::Particle particle(sf::seconds(1.5f));
    particle.position = m_position;
    particle.textureIndex = m_index;

    for (int i=0; i<nr_fragments; ++i){
        using thor::random;

        thor::PolarVector2f velocity(random(100.f, 200.f), random(0.f, 360.f));
        particle.velocity = velocity;
        system.emitParticle(particle);
    }
}



