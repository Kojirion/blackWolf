#pragma once
#include <Thor/Input/ActionMap.hpp>

enum class Action{
    Close,
    Resize,
    Scroll,
    Tab
};

using Actions = thor::ActionMap<Action>;
using CallbackSystem = Actions::CallbackSystem;

