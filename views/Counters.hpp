#ifndef COUNTERS_HPP
#define COUNTERS_HPP
#include <SFGUI/Label.hpp>
#include <SFGUI/Box.hpp>
#include "../BlackWolf.hpp"
#include "../Entity.hpp"

class Counters : private Entity
{
public:
    Counters();

    sfg::Widget::Ptr getView();

private:
    sfg::Box::Ptr box;
    sfg::Label::Ptr repetitionCounter;
    sfg::Label::Ptr fiftyMoveRuleCounter;
};

#endif // COUNTERS_HPP
