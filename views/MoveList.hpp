#ifndef MOVEVIEW_H
#define MOVEVIEW_H
#include <SFGUI/ScrolledWindow.hpp>
#include <SFGUI/Table.hpp>
#include "../Entity.hpp"

struct Move;
struct Square;

class MoveList : private Entity
{
private:
    sfg::ScrolledWindow::Ptr moveListWindow;
    sfg::Table::Ptr moveList;

    std::string colToString(const int col) const;
    std::string moveToString(const Move& move) const;
    std::string cellToString(const Square& square) const;

    void autoscroll();

    int plyCount;
public:
    MoveList();

    void addMove(const Move &move);

    sfg::Widget::Ptr getView();

    void reset();
};

#endif // MOVEVIEW_H
