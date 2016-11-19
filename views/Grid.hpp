#pragma once
#include <SFML/System/Vector2.hpp>

struct Square;

class Grid {
public:
    Grid(int squareSide, int margin);
    sf::Vector2f centerOffset() const;
    Square toSquare(const sf::Vector2f& coords) const;
    sf::Vector2f toCoords(const Square& square) const;
    float boardSide() const;
    void flip();
    bool isFlipped() const;

private:
    float squareSide;
    float margin;
    bool flipped;
};
