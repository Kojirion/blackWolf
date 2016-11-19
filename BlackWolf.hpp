#pragma once
#include <ostream>
#include <SFML/System/Vector2.hpp>
#include "Common/Color.hpp"



struct Piece{
    enum class Type{
        Rook = 0,
        Knight,
        Bishop,
        Queen,
        King,
        Pawn,
        Shadow,
        None
    };

    Color color;
    Type type;
};

const Piece noPiece = {Color::None, Piece::Type::None};
bool operator==(const Piece& lhs, const Piece& rhs);

struct Square{
    int row;
    int col;
};

bool operator<(const Square& lhs, const Square& rhs);
bool operator==(const Square& lhs, const Square& rhs);

std::ostream& operator<<(std::ostream& stream, const Square& square);

struct Move{
    Square square_1;
    Square square_2;
};

bool operator==(const Move& lhs, const Move& rhs);
std::ostream& operator<<(std::ostream& stream, const Move& move);

struct PieceToTexPos {
    sf::Vector2f operator()(const Piece& piece) const;

    int whiteIndex, blackIndex;
};
