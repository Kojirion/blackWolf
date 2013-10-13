#ifndef STATUSVIEW_H
#define STATUSVIEW_H
#include <SFGUI/Label.hpp>
#include "../BlackWolf.hpp"

class Status
{
private:
    sfg::Label::Ptr statusLabel;
public:
    Status();
    void setToPlay(Color side); //set the status to denote side to play
    void setResult(Color winner); //set status to show winner

    sfg::Widget::Ptr getView();
};

#endif // STATUSVIEW_H
