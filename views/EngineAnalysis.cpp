#include "EngineAnalysis.hpp"
#include "../Engine.hpp"
#include <boost/cast.hpp>

EngineAnalysis::EngineAnalysis():
    label(sfg::Label::Create())
{
    messages.connect("engineOutput", [this](const Message& message){
        const EngineOutput* received = boost::polymorphic_downcast<const EngineOutput*>(&message);
        label->SetText(received->output);
    });
}

sfg::Widget::Ptr EngineAnalysis::getWidget() const
{
    return label;
}
