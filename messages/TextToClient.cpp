#include "TextToClient.hpp"

namespace Messages{

TextToClient::TextToClient(const std::string& text):
    Message(ID::TextToClient),
    text(text)
{

}

}
