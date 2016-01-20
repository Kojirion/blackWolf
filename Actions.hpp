#pragma once
#include <Thor/Input/ActionMap.hpp>

enum class Action{
    Close,
    Resize,
    Scroll
};

using Actions = thor::ActionMap<Action>;
using CallbackSystem = Actions::CallbackSystem;

