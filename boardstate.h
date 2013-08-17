#ifndef BOARDSTATE_H
#define BOARDSTATE_H
#include <SFGUI/Window.hpp>

class boardState
{
private:
    sfg::Window::Ptr window;

public:
    boardState(sfg::Window::Ptr theWindow);
};

#endif // BOARDSTATE_H
