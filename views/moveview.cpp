#include "moveview.h"
#include <SFGUI/Label.hpp>

moveView::moveView():
    moveListWindow(sfg::ScrolledWindow::Create()),
    moveList(sfg::Table::Create())
{
    sfg::Label::Ptr dummyLabel(sfg::Label::Create());
    moveList->SetColumnSpacings(0.f);
    moveList->Attach(dummyLabel,{0,0,1,1});
    moveList->SetColumnSpacing(0,10.f);
    moveList->Remove(dummyLabel);

    moveListWindow->SetRequisition(sf::Vector2f(110.f,0.f));
    moveListWindow->SetScrollbarPolicy( sfg::ScrolledWindow::HORIZONTAL_NEVER | sfg::ScrolledWindow::VERTICAL_AUTOMATIC );
    moveListWindow->AddWithViewport(moveList);
}

void moveView::addMove(const int row1, const int col1, const int row2, const int col2, const int plyCounter)
{
    sfg::Label::Ptr newMove(sfg::Label::Create(moveToString(row1,col1,row2,col2)));
    const unsigned int plyPairsCount = plyCounter/2;
    const unsigned int plyRemainder = (plyCounter)%2;
    moveList->Attach(newMove,{plyRemainder,plyPairsCount,1,1});
    autoscroll();
}

sfg::Widget::Ptr moveView::getView()
{
    return moveListWindow;
}

void moveView::reset()
{
    moveList->RemoveAll();
}

std::string moveView::colToString(const int col) const
{
    switch (col) {
    case 0:
        return "a";
    case 1:
        return "b";
    case 2:
        return "c";
    case 3:
        return "d";
    case 4:
        return "e";
    case 5:
        return "f";
    case 6:
        return "g";
    case 7:
        return "h";
    default:
        return "-"; //appease compiler
    }
}

std::string moveView::moveToString(const int row1, const int col1, const int row2, const int col2) const
{
    return (cellToString(row1,col1) + "-" + cellToString(row2,col2));
}

std::string moveView::cellToString(const int row, const int col) const
{
    return (colToString(col) + std::to_string(row+1));
}

void moveView::autoscroll()
{
    sfg::Adjustment::Ptr toAdjust(moveListWindow->GetVerticalAdjustment());
    toAdjust->SetValue(toAdjust->GetUpper());
}
