#include "statusview.h"

statusView::statusView():
    statusLabel(sfg::Label::Create())
{
}

void statusView::setToPlay(Color side)
{
    if (side == Color::White) statusLabel->SetText("White to play");
    else statusLabel->SetText("Black to play");
}


void statusView::setResult(Color winner)
{
    if (winner == Color::White) statusLabel->SetText("White wins!");
    else if (winner == Color::Black) statusLabel->SetText("Black wins!");
    else statusLabel->SetText("Draw!");
}

sfg::Widget::Ptr statusView::getView()
{
    return statusLabel;
}
