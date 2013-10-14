#include "MoveList.hpp"
#include <SFGUI/Label.hpp>
#include "../BlackWolf.hpp"
#include <boost/cast.hpp>
#include "../controller/BoardMaster.hpp"

MoveList::MoveList():
    moveListWindow(sfg::ScrolledWindow::Create()),
    moveList(sfg::Table::Create()),
    plyCount(0)
{
    sfg::Label::Ptr dummyLabel(sfg::Label::Create());
    moveList->SetColumnSpacings(0.f);
    moveList->Attach(dummyLabel,{0,0,1,1});
    moveList->SetColumnSpacing(0,10.f);
    moveList->Remove(dummyLabel);

    moveListWindow->SetRequisition(sf::Vector2f(110.f,0.f));
    moveListWindow->SetScrollbarPolicy( sfg::ScrolledWindow::HORIZONTAL_NEVER | sfg::ScrolledWindow::VERTICAL_AUTOMATIC );
    moveListWindow->AddWithViewport(moveList);

    messageSystem.connect("moveMade", [this](const Message& message){
        const MoveMessage* received = boost::polymorphic_downcast<const MoveMessage*>(&message);
        addMove(received->move.getMove());
    });
}

void MoveList::addMove(const Move& move)
{
    sfg::Label::Ptr newMove(sfg::Label::Create(moveToString(move)));
    const unsigned int plyPairsCount = plyCount/2;
    const unsigned int plyRemainder = (plyCount)%2;
    moveList->Attach(newMove,{plyRemainder,plyPairsCount,1,1});
    autoscroll();
    plyCount++;
}

sfg::Widget::Ptr MoveList::getView()
{
    return moveListWindow;
}

void MoveList::reset()
{
    plyCount = 0;
    moveList->RemoveAll();
}

std::string MoveList::colToString(const int col) const
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

std::string MoveList::moveToString(const Move &move) const
{
    return (cellToString(move.square_1) + "-" + cellToString(move.square_2));
}

std::string MoveList::cellToString(const Square &square) const
{
    return (colToString(square.col) + std::to_string(square.row+1));
}

void MoveList::autoscroll()
{
    sfg::Adjustment::Ptr toAdjust(moveListWindow->GetVerticalAdjustment());
    toAdjust->SetValue(toAdjust->GetUpper());
}
