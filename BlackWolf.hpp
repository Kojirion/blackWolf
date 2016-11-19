#pragma once

#include <SFML/System/Vector2.hpp>


class Piece;

struct PieceToTexPos {
    sf::Vector2f operator()(const Piece& piece) const;

    int whiteIndex, blackIndex;
};
