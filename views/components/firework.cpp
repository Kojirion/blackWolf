#include "firework.h"

const sf::Time FireworkEmitter::tailDuration = sf::seconds(1.5f);
const sf::Time	FireworkEmitter::explosionDuration = sf::seconds(0.2f);

FireworkEmitter::FireworkEmitter(sf::Vector2f position): mAccumulatedTime(sf::Time::Zero)
  , mPosition(position)
  , mColor(sf::Color(255,69,0))
{
}

void FireworkEmitter::operator ()(thor::EmissionAdder &system, sf::Time dt)
{
    const sf::Time tailInterval = explosionDuration / tailsPerExplosion;

    // Accumulate passed time. If enough time has passed (tailInterval), emit a new tail and decrease accumulator.
    mAccumulatedTime += dt;
    while (mAccumulatedTime - tailInterval > sf::Time::Zero)
    {
        emitTail(system);
        mAccumulatedTime -= tailInterval;
    }
}

void FireworkEmitter::emitTail(thor::EmissionAdder &system)
{
    // Create initial direction with random vector length and angle
    thor::PolarVector2f velocity(thor::random(30.f, 70.f), thor::random(0.f, 360.f));

    // Create particle at position of explosion, with emitter-specific color and at 80% initial scale
    thor::Particle particle(tailDuration);
    particle.position = mPosition;
    particle.color = mColor;
    particle.scale *= 0.8f;

    // A tail contains 25 particles with different speeds and scales.
    // The largest particles move fastest, leading to a comet-like tail effect.
    for (sf::Int64 i = 0; i < 25; ++i)
    {
        // Decrease scale continuously
        particle.scale *= 0.95f;

        // Decrease speed continuously
        velocity.r *= 0.96f;
        particle.velocity = velocity;

        // Add adapted particle to particle system
        system.addParticle(particle);
    }
}
