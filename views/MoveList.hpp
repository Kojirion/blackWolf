#pragma once
#include <SFGUI/ScrolledWindow.hpp>
#include <SFGUI/Table.hpp>
#include "../Entity.hpp"

class MoveList : private Entity
{
private:
    sfg::ScrolledWindow::Ptr moveListWindow;
    sfg::Table::Ptr moveList;

    void autoscroll();

    int plyCount;
public:
    MoveList();

    void addMove(const std::string &move);

    sfg::Widget::Ptr getView();

    void reset();
};
