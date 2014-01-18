#include "Counters.hpp"
#include <boost/cast.hpp>

Counters::Counters():
    box(sfg::Box::Create(sfg::Box::Orientation::VERTICAL)),
    repetitionCounter(sfg::Label::Create("Repetition counter: ")),
    fiftyMoveRuleCounter(sfg::Label::Create("Fifty move rule counter: "))
{
    box->Pack(repetitionCounter);
    box->Pack(fiftyMoveRuleCounter);

    messages.connect("count", [this](const Message& message){
        const CountMessage* received = boost::polymorphic_downcast<const CountMessage*>(&message);
        repetitionCounter->SetText("Repetition counter: " + std::to_string(received->repetitions));
        fiftyMoveRuleCounter->SetText("Fifty move rule counter: " +  std::to_string(received->fiftyMoves));
    });
}

sfg::Widget::Ptr Counters::getView()
{
    return box;
}
