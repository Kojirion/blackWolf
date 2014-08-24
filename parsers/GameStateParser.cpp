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

PromotionMap promotionMap;


GameStateParser::GameStateParser() : base_type(start)
{
    piece = char_("rnbqkpRNBQKP") | '-';
    row = repeat(8)[piece];
    color = lit('W') | lit('B');
    double_advance = "-1" | digit;
    castling_right = lit('0') | lit('1');
    irreversible_moves = uint_;
    game_id = uint_;
    name = +alnum;
    //verbose_move = );
                   //| lit("o-o") | lit("o-o-o");
    time_taken = '(' >> uint_ >> ':' >> uint_ >> ')';
    relation = lit("-3") | lit("-2") | lit("2") | lit("-1") | lit("1") | lit("0");
    pretty_move = +alnum;
    start = "<12> " >> omit[repeat(8)[row >> ' '] >> color >> ' ' >> double_advance >> ' ' >> repeat(4)[castling_right >> ' '] >> irreversible_moves >> ' '
                    >> game_id >> ' ' >> name >> ' ' >> name >> ' ' >> relation >> ' ' >> repeat(4)[uint_ >> ' ']] >> int_>>' '>>int_ >> omit[' ' >> uint_
                    >> ' '] >> omit[alpha >> '/'] >> square >> omit['-'] >> square /*>> -(omit[alpha>>digit>>'='] >> promotionMap)*/
                    >> omit[' ' >> time_taken >> ' ' >> pretty_move >> ' ' >> repeat(2)[uint_ >> ' '] >> uint_];
}
