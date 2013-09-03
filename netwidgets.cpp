#include "netwidgets.h"

netWidgets::netWidgets():
    window(sfg::Window::Create()),
    chatAreaLayout(sfg::Box::Create(sfg::Box::VERTICAL)),
    chatEntry(sfg::Entry::Create()),
    chatWindow(sfg::ScrolledWindow::Create())
{
}
