#ifndef STATUSVIEW_H
#define STATUSVIEW_H
#include <SFGUI/Label.hpp>
#include "../BlackWolf.hpp"
#include "../Entity.hpp"

class Status : private Entity
{
private:
    sfg::Label::Ptr statusLabel;
    void setToPlay(Color side);
    void setResult(Color winner);
public:
    Status();    

    sfg::Widget::Ptr getView();
};

#endif // STATUSVIEW_H
