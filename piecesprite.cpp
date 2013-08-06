#include "piecesprite.h"
#include <boost/math/special_functions/sign.hpp>

pieceSprite::pieceSprite(const sf::Texture &texture, int theType):
    sf::Sprite(texture), pieceType(theType)
{

}

pieceSprite::pieceSprite(const sf::Texture &texture, const sf::Vector2f &position, int theSide):
    sf::Sprite(texture), side(theSide)
{
    sf::Sprite::setPosition(position);
}

int pieceSprite::getSide() const
{
    return boost::math::sign(pieceType)
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
