#include "TextReady.hpp"

namespace Messages{

TextReady::TextReady(const std::string& text):
    Message(ID::TextReady),
    text(text)
{

}

}
