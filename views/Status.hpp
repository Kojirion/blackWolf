#pragma once
#include <SFGUI/Label.hpp>
#include "../messages/Entity.hpp"

enum class Color;

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
