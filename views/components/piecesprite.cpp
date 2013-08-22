#include "piecesprite.h"
#include <boost/math/special_functions/sign.hpp>

/*pieceSprite::pieceSprite(const sf::Texture &texture, int theType):
    sf::Sprite(texture), pieceType(theType)
{

}*/

pieceSprite::pieceSprite(const sf::Texture &texture, const sf::Vector2f &position, int theType, int theId):
    sf::Sprite(texture), pieceType(theType), id(theId)
{
    sf::Sprite::setPosition(position);
}

bool pieceSprite::operator <(const pieceSprite &that) const
{
    return (id<that.id);
}

int pieceSprite::getSide() const
{
    return boost::math::sign(pieceType);
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
