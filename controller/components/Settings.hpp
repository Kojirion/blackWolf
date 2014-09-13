#pragma once
#include <SFGUI/Window.hpp>
#include <SFGUI/Desktop.hpp>
#include <SFGUI/Button.hpp>
#include <SFGUI/RadioButton.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <array>
#include <boost/signals2.hpp>
#include <boost/property_tree/ptree.hpp>

class Settings : private sf::NonCopyable
{
private:
    sfg::Window::Ptr window;
    sfg::Desktop& desktop;
    sfg::Button::Ptr closeButton;

    std::array<std::string,12> colors;
    std::array<sfg::RadioButton::Ptr,12> whiteButtons;
    std::array<sfg::RadioButton::Ptr,12> blackButtons;

    std::array<std::string,3> boardColors;
    std::array<sfg::RadioButton::Ptr,3> boardButtons;

    void requestClose();

    std::string toSetWhite;
    std::string toSetBlack;
    std::string toSetBoard;

public:
    Settings(sfg::Desktop& theDesktop);
    void enable(bool doEnable = true);
    sfg::Widget::Ptr getWidget();

    boost::signals2::signal<void (std::string, std::string, std::string)> settingsDone;

    void setTree(const boost::property_tree::ptree pt);
};
