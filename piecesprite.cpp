#include "piecesprite.h"

pieceSprite::pieceSprite(const sf::Texture &texture, int theSide):
    sf::Sprite(texture), side(theSide)
{

}

pieceSprite::pieceSprite(const sf::Texture &texture, const sf::Vector2f &position, int theSide):
    sf::Sprite(texture), side(theSide)
{
    sf::Sprite::setPosition(position);
}

int pieceSprite::getSide() const
{
    return side;
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
