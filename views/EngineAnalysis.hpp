#ifndef ENGINEANALYSIS_HPP
#define ENGINEANALYSIS_HPP
#include <SFGUI/Label.hpp>
#include <SFGUI/Box.hpp>
#include <SFGUI/ScrolledWindow.hpp>
#include "../Entity.hpp"

class EngineAnalysis : private Entity
{
public:
    EngineAnalysis();

    sfg::Widget::Ptr getWidget() const;

private:
    sfg::Box::Ptr outputLayout;
    sfg::ScrolledWindow::Ptr outputWindow;
};

#endif // ENGINEANALYSIS_HPP
