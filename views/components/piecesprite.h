#ifndef PIECESPRITE_H
#define PIECESPRITE_H
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Rect.hpp>
#include "../../blackWolf.h"
#include <array>

class pieceSprite
{
private:
    Unit m_type;
    int m_id;

    mutable std::array<sf::Vertex, 4> m_quad;

    sf::Vector2f typeToTexPos(const Unit& type) const;

public:    
    pieceSprite(const sf::Vector2f &position, Unit type, int id);

    bool operator<(const pieceSprite &that) const;

    void appendQuadTo(std::vector<sf::Vertex>& toAppendTo) const;

    Color getColor() const;
    bool contains(const sf::Vector2f &point) const;

    void setPosition(const sf::Vector2f& position) const;

    const sf::Vector2f& getPosition() const;
};

#endif // PIECESPRITE_H
