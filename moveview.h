#ifndef MOVEVIEW_H
#define MOVEVIEW_H
#include <SFGUI/ScrolledWindow.hpp>
#include <SFGUI/Table.hpp>

class moveView
{
private:
    sfg::ScrolledWindow::Ptr moveListWindow;
    sfg::Table::Ptr moveList;

    std::string colToString(const int col) const;
    std::string moveToString(const int row1, const int col1, const int row2, const int col2) const;
    std::string cellToString(const int row, const int col) const;
public:
    moveView();

    void addMove(const int row1, const int col1, const int row2, const int col2, const int plyCounter);

    sfg::Widget::Ptr getView();

    void reset();
};

#endif // MOVEVIEW_H
