#ifndef PIECESPRITE_H
#define PIECESPRITE_H
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Rect.hpp>
#include "../../blackWolf.h"

class pieceSprite : public sf::Sprite
{
private:
    Unit m_type;
    int m_id;
public:
    //pieceSprite(const sf::Texture &texture, int theType);
    pieceSprite(const sf::Texture &texture, const sf::Vector2f &position, Unit type, int id);

    bool operator<(const pieceSprite &that) const;

    Color getColor() const;
    bool contains(const sf::Vector2f &point) const;
};

#endif // PIECESPRITE_H
