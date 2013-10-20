#include "Status.hpp"
#include <boost/cast.hpp>

Status::Status():
    statusLabel(sfg::Label::Create())
{
    messages.connect("moveMade", [this](const Message& message){
        const MoveMessage* received = boost::polymorphic_downcast<const MoveMessage*>(&message);
        setToPlay(received->move.getNewBoard().getTurnColor());
    });

    messages.connect("newGame", [this](const Message& message){
        setToPlay(Color::White);
    });

    messages.connect("endGame", [this](const Message& message){
        const EndGameMessage* received = boost::polymorphic_downcast<const EndGameMessage*>(&message);
        setResult(received->result);
    });
}

void Status::setToPlay(Color side)
{
    if (side == Color::White) statusLabel->SetText("White to play");
    else statusLabel->SetText("Black to play");
}


void Status::setResult(Color winner)
{
    if (winner == Color::White) statusLabel->SetText("White wins!");
    else if (winner == Color::Black) statusLabel->SetText("Black wins!");
    else statusLabel->SetText("Draw!");
}

sfg::Widget::Ptr Status::getView()
{
    return statusLabel;
}
