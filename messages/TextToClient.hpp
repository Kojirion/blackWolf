#pragma once
#include "Message.hpp"

namespace Messages {

struct TextToClient : public Message {
    TextToClient(const std::string& text);
    std::string text;
};

}
