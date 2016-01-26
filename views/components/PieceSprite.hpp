#ifndef PIECESPRITE_H
#define PIECESPRITE_H
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Rect.hpp>
#include "../../BlackWolf.hpp"
#include <array>

class PieceSprite
{
public:    
    PieceSprite(const sf::Vector2f& position, const Piece& piece, const PieceToTexPos& pieceToTexPos, int id);

    bool operator<(const PieceSprite &that) const;

    const Piece& getPiece() const;

    void appendQuadTo(std::vector<sf::Vertex>& toAppendTo) const;

    Color getColor() const;
    bool contains(const sf::Vector2f &point) const;

    void setPosition(const sf::Vector2f& position) const;

    const sf::Vector2f& getPosition() const;    

private:
    Piece m_piece;
    int m_id;

    mutable std::array<sf::Vertex, 4> m_quad;
};

#endif // PIECESPRITE_H
