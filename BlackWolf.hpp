#pragma once
#include <ostream>
#include <SFML/System/Vector2.hpp>

enum class Color{
    None = 0,
    White = 1 << 0,
    Black = 1 << 1,
    Both = White | Black
};

bool operator &(const Color& lhs, const Color& rhs);
Color operator!(Color rhs);
int sign(Color color);

enum class Piece{    
    Rook = 0,
    Knight = 1,
    Bishop = 2,
    Queen = 3,
    King = 4,
    Pawn = 5,
    Shadow = 6,
    None = 7
};

struct Unit{
    Color color;
    Piece piece;
};

const Unit noPiece = {Color::None, Piece::None};
bool operator==(const Unit& lhs, const Unit& rhs);

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

enum class Side {
    King = 0,
    Queen = 1
};

struct Castle{
    Color color;
    Side side;
};

bool operator<(const Castle& lhs, const Castle& rhs);

//namespace sf { class Vector2f; }

sf::Vector2f typeToTexPos(const Unit& type);
