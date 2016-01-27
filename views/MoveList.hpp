#pragma once
#include <SFGUI/ScrolledWindow.hpp>
#include <SFGUI/Table.hpp>
#include "../messages/Entity.hpp"

class MoveList : private Messages::Registrant
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
