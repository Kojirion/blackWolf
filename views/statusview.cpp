#include "statusview.h"

statusView::statusView():
    statusLabel(sfg::Label::Create())
{
}

void statusView::setToPlay(const bw side)
{
    if (side == bw::White) statusLabel->SetText("White to play");
    else statusLabel->SetText("Black to play");
}


void statusView::setResult(const bw winner)
{
    if (winner == bw::White) statusLabel->SetText("White wins!");
    else if (winner == bw::Black) statusLabel->SetText("Black wins!");
    else statusLabel->SetText("Draw!");
}

sfg::Widget::Ptr statusView::getView()
{
    return statusLabel;
}
