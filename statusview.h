#ifndef STATUSVIEW_H
#define STATUSVIEW_H
#include <SFGUI/Label.hpp>
#include "blackWolf.h"

class statusView
{
private:
    sfg::Label::Ptr statusLabel;
public:
    statusView();
    void setToPlay(const bw side); //set the status to denote side to play
    void setResult(const bw winner); //set status to show winner
};

#endif // STATUSVIEW_H
