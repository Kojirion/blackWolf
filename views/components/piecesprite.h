#ifndef PIECESPRITE_H
#define PIECESPRITE_H
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Rect.hpp>

class pieceSprite : public sf::Sprite
{
private:
    int pieceType;
    int id;
public:
    //pieceSprite(const sf::Texture &texture, int theType);
    pieceSprite(const sf::Texture &texture, const sf::Vector2f &position, int theSide, int theId);

    bool operator<(const pieceSprite &that) const;

    int getSide() const;
    bool contains(const sf::Vector2f &point) const;
};

#endif // PIECESPRITE_H
