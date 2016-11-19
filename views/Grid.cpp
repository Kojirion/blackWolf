#include "Grid.hpp"
#include "../Common/Square.hpp"
#include <cmath>

Grid::Grid(int squareSide, int margin):
    squareSide(squareSide),
    margin(margin),
    flipped(false)
{

}

sf::Vector2f Grid::centerOffset() const{
    return {squareSide/2.f, squareSide/2.f};
}

Square Grid::toSquare(const sf::Vector2f &coords) const{
    if (flipped)
        return {static_cast<int>(std::floor((coords.y+7*squareSide-7*squareSide-margin)/(2*squareSide-squareSide))),
                    static_cast<int>(std::ceil((coords.x-7*squareSide-margin)/(squareSide-2*squareSide)))};
    else
        return {static_cast<int>(std::ceil((coords.y-7*squareSide-margin)/(-squareSide))),
                    static_cast<int>(std::floor((coords.x-margin)/squareSide))};
}

sf::Vector2f Grid::toCoords(const Square &square) const {
    sf::Vector2f result{margin + squareSide*square.col,
                8*squareSide + margin - squareSide*(square.row+1)};

    if (flipped)
        result += {squareSide* (7 - 2*square.col), -squareSide * (7 - 2*square.row)};

    return result;
}

float Grid::boardSide() const {
    return 8*squareSide + 2*margin;
}

void Grid::flip(){
    flipped = !flipped;
}

bool Grid::isFlipped() const {
    return flipped;
}
