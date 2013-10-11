#include "piecesprite.h"
#include <boost/math/special_functions/sign.hpp>

/*pieceSprite::pieceSprite(const sf::Texture &texture, int theType):
    sf::Sprite(texture), pieceType(theType)
{

}*/

pieceSprite::pieceSprite(const sf::Texture &texture, const sf::Vector2f &position, Unit type, int id):
    sf::Sprite(texture), m_type(type), m_id(id)
{
    sf::Sprite::setPosition(position);
}

bool pieceSprite::operator <(const pieceSprite &that) const
{
    return (m_id<that.m_id);
}

Color pieceSprite::getColor() const
{
    return m_type.color;
}

bool pieceSprite::contains(const sf::Vector2f &point) const
{
    return getGlobalBounds().contains(point);
}

/*void pieceSprite::setPosition(float x, float y)
{
    sf::Sprite::setPosition(x,y);
    boundingRect = sf::Sprite::get
}*/
