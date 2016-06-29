#pragma once
#include "Message.hpp"


namespace Messages {

struct TextReady : public Message {
    TextReady(const std::string& text);
    std::string text;
};

}
