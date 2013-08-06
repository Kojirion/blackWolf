#ifndef PIECESPRITE_H
#define PIECESPRITE_H
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Rect.hpp>

class pieceSprite : public sf::Sprite
{
private:
    int side;
public:
    pieceSprite(const sf::Texture &texture, int theSide);
    pieceSprite(const sf::Texture &texture, const sf::Vector2f &position, int theSide);
    //sf::FloatRect boundingRect;

    //void setPosition(float x, float y);

    int getSide() const;
    bool contains(const sf::Vector2f &point) const;
};

#endif // PIECESPRITE_H
