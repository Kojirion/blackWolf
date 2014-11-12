#include "PieceSprite.hpp"
#include <boost/math/special_functions/sign.hpp>
#include <boost/assert.hpp>

PieceSprite::PieceSprite(const sf::Vector2f &position, Unit type, int id):
    m_type(type), m_id(id)
{
    setPosition(position);

    m_quad[0].texCoords = typeToTexPos(m_type);
    m_quad[1].texCoords = m_quad[0].texCoords + sf::Vector2f(50.f, 0);
    m_quad[2].texCoords = m_quad[0].texCoords + sf::Vector2f(50.f, 50.f);
    m_quad[3].texCoords = m_quad[0].texCoords + sf::Vector2f(0, 50.f);
}

bool PieceSprite::operator <(const PieceSprite &that) const
{
    return (m_id<that.m_id);
}

const Unit &PieceSprite::getUnit() const
{
    return m_type;
}

void PieceSprite::appendQuadTo(std::vector<sf::Vertex> &toAppendTo) const
{
    toAppendTo.insert(toAppendTo.end(), m_quad.begin(), m_quad.end());
}

Color PieceSprite::getColor() const
{
    return m_type.color;
}

bool PieceSprite::contains(const sf::Vector2f &point) const
{
    sf::Vector2f topLeft = m_quad[0].position;
    sf::Vector2f lowRight = m_quad[2].position;

    return (point.x >= topLeft.x) && (point.x <= lowRight.x)
            && (point.y >= topLeft.y) && (point.y <= lowRight.y);
}

void PieceSprite::setPosition(const sf::Vector2f &position) const
{
    m_quad[0].position = position;
    m_quad[1].position = m_quad[0].position + sf::Vector2f(50.f, 0);
    m_quad[2].position = m_quad[0].position + sf::Vector2f(50.f, 50.f);
    m_quad[3].position = m_quad[0].position + sf::Vector2f(0, 50.f);
}

const sf::Vector2f &PieceSprite::getPosition() const
{
    return m_quad[0].position;
}
