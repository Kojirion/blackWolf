#ifndef FIREWORK_H
#define FIREWORK_H
#include <SFML/Graphics.hpp>

#include <Thor/Particles/ParticleSystem.hpp>
#include <Thor/Particles/EmissionAdder.hpp>
#include <Thor/Vectors/PolarVector.hpp>
#include <Thor/Math/Random.hpp>
#include <Thor/Time/CallbackTimer.hpp>

#include <array>
#include <memory>


class FireworkEmitter
{
public:
    static const sf::Int64	tailsPerExplosion = 8;
    static const sf::Time	tailDuration;
    static const sf::Time	explosionDuration;

    explicit FireworkEmitter(sf::Vector2f position);


    void operator() (thor::EmissionAdder& system, sf::Time dt);

private:
    void emitTail(thor::EmissionAdder& system);

    sf::Time	mAccumulatedTime;
    sf::Vector2f	mPosition;
    sf::Color	mColor;
};

class FireworkAffector
{
public:
    void operator() (thor::Particle& particle, sf::Time dt)
    {
        // Let particles continuously fade out (particles with smaller scale have already lower alpha value at beginning)
        particle.color.a = static_cast<sf::Uint8>(256 * thor::getRemainingRatio(particle) * particle.scale.x);
    }
};

#endif // FIREWORK_H
