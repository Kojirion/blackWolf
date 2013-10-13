#include "piecesprite.h"
#include <boost/math/special_functions/sign.hpp>
#include <boost/assert.hpp>

pieceSprite::pieceSprite(const sf::Vector2f &position, Unit type, int id):
    m_type(type), m_id(id)
{
    setPosition(position);

    m_quad[0].texCoords = typeToTexPos(m_type);
    m_quad[1].texCoords = m_quad[0].texCoords + sf::Vector2f(50.f, 0);
    m_quad[2].texCoords = m_quad[0].texCoords + sf::Vector2f(50.f, 50.f);
    m_quad[3].texCoords = m_quad[0].texCoords + sf::Vector2f(0, 50.f);
}

bool pieceSprite::operator <(const pieceSprite &that) const
{
    return (m_id<that.m_id);
}

void pieceSprite::appendQuadTo(std::vector<sf::Vertex> &toAppendTo) const
{
    toAppendTo.insert(toAppendTo.end(), m_quad.begin(), m_quad.end());
}

Color pieceSprite::getColor() const
{
    return m_type.color;
}

bool pieceSprite::contains(const sf::Vector2f &point) const
{
    sf::Vector2f topLeft = m_quad[0].position;
    sf::Vector2f lowRight = m_quad[2].position;

    return (point.x >= topLeft.x) && (point.x <= lowRight.x)
            && (point.y >= topLeft.y) && (point.y <= lowRight.y);
}

void pieceSprite::setPosition(const sf::Vector2f &position) const
{
    m_quad[0].position = position;
    m_quad[1].position = m_quad[0].position + sf::Vector2f(50.f, 0);
    m_quad[2].position = m_quad[0].position + sf::Vector2f(50.f, 50.f);
    m_quad[3].position = m_quad[0].position + sf::Vector2f(0, 50.f);
}

const sf::Vector2f &pieceSprite::getPosition() const
{
    return m_quad[0].position;
}

sf::Vector2f pieceSprite::typeToTexPos(const Unit &type) const
{
    BOOST_ASSERT_MSG((type.color==Color::Black)||(type.color==Color::White), "Invalid piece color");
    BOOST_ASSERT_MSG((type.piece!=Piece::None)&&(type.piece!=Piece::Shadow), "Invalid piece type");


    const sf::Vector2f offset(20.f, 20.f);
    const int side = 50;

    if (type.piece == Piece::Pawn){
        if (type.color == Color::White)
            return (offset + sf::Vector2f(0, 6*side));
        else return (offset + sf::Vector2f(0, side));
    }

    if (type.color == Color::White)
        return (offset + sf::Vector2f(static_cast<int>(type.piece)*side, 7*side));
    else
        return (offset + sf::Vector2f(static_cast<int>(type.piece)*side, 0));
}
