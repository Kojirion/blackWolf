#ifndef MOVEVIEW_H
#define MOVEVIEW_H
#include <SFGUI/ScrolledWindow.hpp>
#include <SFGUI/Table.hpp>

class Move;
class Square;

class moveView
{
private:
    sfg::ScrolledWindow::Ptr moveListWindow;
    sfg::Table::Ptr moveList;

    std::string colToString(const int col) const;
    std::string moveToString(const Move& move) const;
    std::string cellToString(const Square& square) const;

    void autoscroll();
public:
    moveView();

    void addMove(const Move &move, const int plyCounter);

    sfg::Widget::Ptr getView();

    void reset();
};

#endif // MOVEVIEW_H
