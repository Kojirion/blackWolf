#ifndef PIECESPRITE_H
#define PIECESPRITE_H
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Rect.hpp>
#include "../../blackWolf.h"

class pieceSprite : public sf::Sprite
{
private:
    bw pieceType;
    int id;
public:
    //pieceSprite(const sf::Texture &texture, int theType);
    pieceSprite(const sf::Texture &texture, const sf::Vector2f &position, bw theType, int theId);

    bool operator<(const pieceSprite &that) const;

    bw getSide() const;
    bool contains(const sf::Vector2f &point) const;
};

#endif // PIECESPRITE_H
