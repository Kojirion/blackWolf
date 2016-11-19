#pragma once
#include <Thor/Vectors/PolarVector2.hpp>
#include <Thor/Particles/Affectors.hpp>
#include <Thor/Math.hpp>
#include "../../Common/Piece.hpp"

extern const int nr_fragments_side;
extern const int nr_fragments;

extern const Piece IntToPiece[12];

int UnitToInt(const Piece& unit);

namespace thor { class EmissionInterface; }

class Emitter {
public:
    Emitter(const sf::Vector2f& position, const Piece& unit);


    void operator()(thor::EmissionInterface& system, sf::Time dt);

private:
    sf::Vector2f m_position;
    int m_index;
};
