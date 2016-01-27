#pragma once
#include <SFGUI/Label.hpp>
#include "../BlackWolf.hpp"
#include "../messages/Entity.hpp"

class Status : private Messages::Registrant
{
private:
    sfg::Label::Ptr statusLabel;
    void setToPlay(Color side);
    void setResult(Color winner);
public:
    Status();    

    sfg::Widget::Ptr getView();
};
