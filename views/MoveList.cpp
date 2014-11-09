#include "MoveList.hpp"
#include <SFGUI/Label.hpp>
#include "../BlackWolf.hpp"
#include <boost/cast.hpp>
#include "../controller/Controller.hpp"
#include <SFGUI/Adjustment.hpp>

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

    messages.connect("gameState", [this](const Message& message){
        auto received = boost::polymorphic_downcast<const GameStateMessage*>(&message);
        if (plyCount%2==0){
            if (received->turnColor==Color::White)
                return;
        } else{
            if (received->turnColor==Color::Black)
                return;
        }
        addMove(received->move);
    });

    messages.connect("newGame", [this](const Message& message){
        reset();
    });
}

void MoveList::addMove(const std::string& move)
{
    if (move == "none")
        return;
    const unsigned int plyPairsCount = plyCount/2;
    const unsigned int plyRemainder = (plyCount)%2;
    std::string toAdd;
    if (plyRemainder == 0)
        toAdd = std::to_string(plyPairsCount+1) + ". " + move;
    else
        toAdd = move;
    sfg::Label::Ptr newMove(sfg::Label::Create(toAdd));
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

void MoveList::autoscroll()
{
    sfg::Adjustment::Ptr toAdjust(moveListWindow->GetVerticalAdjustment());
    toAdjust->SetValue(toAdjust->GetUpper());
}
