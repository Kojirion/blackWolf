#ifndef ENGINEANALYSIS_HPP
#define ENGINEANALYSIS_HPP
#include <SFGUI/Label.hpp>
#include "../Entity.hpp"

class EngineAnalysis : private Entity
{
public:
    EngineAnalysis();

    sfg::Widget::Ptr getWidget() const;

private:
    sfg::Label::Ptr label;
};

#endif // ENGINEANALYSIS_HPP
