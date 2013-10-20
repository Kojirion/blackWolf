#ifndef BLACKWOLF_H
#define BLACKWOLF_H

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

struct Move{
    Square square_1;
    Square square_2;
};


#endif // BLACKWOLF_H
