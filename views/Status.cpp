#include "Status.hpp"

Status::Status():
    statusLabel(sfg::Label::Create())
{
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
