#pragma once
#include <string>

namespace Messages {

struct Message{
    Message(const std::string& id);

    virtual ~Message() = default;

    std::string id;
};

std::string getEventId(const Message &message);

}
