#ifndef PIECESPRITE_H
#define PIECESPRITE_H
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Rect.hpp>

class pieceSprite : public sf::Sprite
{
public:
    pieceSprite(const sf::Texture &texture);
    pieceSprite(const sf::Texture &texture, const sf::Vector2f &position);
    //sf::FloatRect boundingRect;

    //void setPosition(float x, float y);

    bool contains(const sf::Vector2f &point);
};

#endif // PIECESPRITE_H
