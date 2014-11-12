#include "GameStateParser.hpp"
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/qi_symbols.hpp>

//<12> rQb-kbnr p--qpppp --n----- -------- -------- -------- PPPP-PPP RNBQKBNR B -1 1 1 1 1 0 171 Wootywoo Kojijay 1 2 12 47 36 118 134 5 P/b7-b8=Q (0:22) b8=Q 1 1 0

using namespace boost::spirit::qi;

struct PromotionMap : symbols<char, Piece>
{
    PromotionMap(){
        add
                ("Q", Piece::Queen)
                ("B", Piece::Bishop)
                ("N", Piece::Knight)
                ("R", Piece::Rook)
                ;
    }

};

struct ColorMap : symbols<char, Color>
{
    ColorMap(){
        add
                ("W", Color::White)
                ("B", Color::Black)
                ;
    }
};



struct PieceMap : symbols<char, Unit>
{
    PieceMap(){
        add
                ("r", Unit{Color::Black, Piece::Rook})
                ("n", Unit{Color::Black, Piece::Knight})
                ("b", Unit{Color::Black, Piece::Bishop})
                ("q", Unit{Color::Black, Piece::Queen})
                ("k", Unit{Color::Black, Piece::King})
                ("p", Unit{Color::Black, Piece::Pawn})
                ("R", Unit{Color::White, Piece::Rook})
                ("N", Unit{Color::White, Piece::Knight})
                ("B", Unit{Color::White, Piece::Bishop})
                ("Q", Unit{Color::White, Piece::Queen})
                ("K", Unit{Color::White, Piece::King})
                ("P", Unit{Color::White, Piece::Pawn})
                ("-", Unit{Color::None, Piece::None})
                ;
    }

};

ColorMap colorMap;
PromotionMap promotionMap;
PieceMap pieceMap;



GameStateParser::GameStateParser() : base_type(start)
{


    row = repeat(8)[pieceMap];
    double_advance = "-1" | digit;
    castling_right = lit('0') | lit('1');
    irreversible_moves = uint_;
    game_id = uint_;
    name = +alnum;
    //verbose_move = );
    //| lit("o-o") | lit("o-o-o");
    time_taken = '(' >> uint_ >> ':' >> uint_ >> ')';
    relation = lit("-3") | lit("-2") | lit("2") | lit("-1") | lit("1") | lit("0");
    pretty_move = /*lit("none") |*/ (+(alnum | char_('=') | char_('+') | char_('-')));
    start = omit["<12> "] >> repeat(8)[row >> ' '] >> colorMap >> omit[' ' >> double_advance >> ' ' >> repeat(4)[castling_right >> ' '] >> irreversible_moves >> ' '
                          >> game_id >> ' ' >> name >> ' ' >> name >> ' ' >> relation >> ' ' >> repeat(4)[uint_ >> ' ']] >> int_>>' '>>int_ >> omit[' ' >> uint_
                          >> ' '] >> ((omit[char_("RNBQKP") >> '/' >> square >> '-'] >> square >> omit[-(lit('=') >> promotionMap)]) | lit("none") | lit("o-o-o") | lit("o-o"))
                          >> omit[' ' >> time_taken >> ' '] >> pretty_move >> omit[' ' >> repeat(2)[uint_ >> ' '] >> uint_];
}
