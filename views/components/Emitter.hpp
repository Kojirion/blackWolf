#pragma once
#include <Thor/Vectors/PolarVector2.hpp>
#include <Thor/Particles/Affectors.hpp>
#include <Thor/Math.hpp>
#include "../../BlackWolf.hpp"

extern const int nr_fragments_side;
extern const int nr_fragments;

extern const Unit IntToUnit[12];

int UnitToInt(const Unit& unit);

namespace thor { class EmissionInterface; }

class Emitter {
public:
    Emitter(const sf::Vector2f& position, const Unit& unit);


    void operator()(thor::EmissionInterface& system, sf::Time dt);

private:
    sf::Vector2f m_position;
    int m_index;
};
