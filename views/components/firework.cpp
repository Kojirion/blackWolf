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

    mAccumulatedTime += dt;
    while (mAccumulatedTime - tailInterval > sf::Time::Zero)
    {
        emitTail(system);
        mAccumulatedTime -= tailInterval;
    }
}

void FireworkEmitter::emitTail(thor::EmissionAdder &system)
{
    thor::PolarVector2f velocity(thor::random(30.f, 70.f), thor::random(0.f, 360.f));

    thor::Particle particle(tailDuration);
    particle.position = mPosition;
    particle.color = mColor;
    particle.scale *= 0.8f;

    for (sf::Int64 i = 0; i < 25; ++i)
    {
        particle.scale *= 0.95f;

        velocity.r *= 0.96f;
        particle.velocity = velocity;        

        system.addParticle(particle);
    }
}
