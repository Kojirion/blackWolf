#pragma once
#include <string>

namespace Messages {

enum class ID{
    GameStart,
    GameState,
    GameEnd,
    TextReady
};

struct Message{
    Message(ID id);

    virtual ~Message() = default;

    ID id;
};

ID getEventId(const Message &message);

}
