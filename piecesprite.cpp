#include "piecesprite.h"

pieceSprite::pieceSprite(const sf::Texture &texture):
    sf::Sprite(texture)
{

}

pieceSprite::pieceSprite(const sf::Texture &texture, const sf::Vector2f &position):
    sf::Sprite(texture)
{
    sf::Sprite::setPosition(position);
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
