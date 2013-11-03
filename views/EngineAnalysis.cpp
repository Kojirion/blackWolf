#include "EngineAnalysis.hpp"
#include "../Engine.hpp"
#include <boost/cast.hpp>

EngineAnalysis::EngineAnalysis():
    outputLayout(sfg::Box::Create(sfg::Box::Orientation::VERTICAL)),
    outputWindow(sfg::ScrolledWindow::Create())
{
    outputWindow->SetRequisition(sf::Vector2f(200.f,100.f));
    outputWindow->SetScrollbarPolicy( sfg::ScrolledWindow::HORIZONTAL_AUTOMATIC | sfg::ScrolledWindow::VERTICAL_AUTOMATIC );
    outputWindow->AddWithViewport(outputLayout);

    messages.connect("engineOutput", [this](const Message& message){
        const EngineOutput* received = boost::polymorphic_downcast<const EngineOutput*>(&message);

        sfg::Container::WidgetsList lines(outputLayout->GetChildren());
        if (lines.size()>4) //if more, pop front
            outputLayout->Remove(lines.front());

        sfg::Label::Ptr toAdd(sfg::Label::Create(received->output));
        toAdd->SetAlignment(sf::Vector2f(0.f,0.f));
        outputLayout->PackEnd(toAdd);
        //autoscroll();
    });
}

sfg::Widget::Ptr EngineAnalysis::getWidget() const
{
    return outputWindow;
}
