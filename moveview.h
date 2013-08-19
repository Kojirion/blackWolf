#ifndef MOVEVIEW_H
#define MOVEVIEW_H
#include <SFGUI/ScrolledWindow.hpp>
#include <SFGUI/Table.hpp>

class moveView
{
private:
    sfg::ScrolledWindow::Ptr moveListWindow;
public:
    moveView();

    void addMove(const int row1, const int col1, const int row2, const int row2);
};

#endif // MOVEVIEW_H
