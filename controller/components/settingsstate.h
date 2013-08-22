#ifndef SETTINGSSTATE_H
#define SETTINGSSTATE_H
#include <SFGUI/Window.hpp>
#include <SFGUI/Desktop.hpp>
#include <SFGUI/Button.hpp>
#include <SFGUI/RadioButton.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <array>
#include <boost/signals2.hpp>

class settingsState : private sf::NonCopyable
{
private:
    sfg::Window::Ptr window;
    sfg::Desktop& desktop;
    sfg::Button::Ptr closeButton;

    std::array<std::string,12> colors;
    std::array<sfg::RadioButton::Ptr,12> whiteButtons;

    void requestClose();

    std::string whitePrefix;
    std::string blackPrefix;
    std::string boardSuffix;

public:
    settingsState(sfg::Desktop& theDesktop);
    void enable(bool doEnable = true);
    sfg::Widget::Ptr getWidget();

    boost::signals2::signal<void (std::string, std::string, std::string)> settingsDone;
};

#endif // SETTINGSSTATE_H
