#pragma once
#include <SFGUI/Label.hpp>

class Clock : public sfg::Label
{
public:
    typedef std::shared_ptr<Clock> Ptr;
    typedef std::shared_ptr<const Clock> PtrConst;

    static Ptr Create();

    void setTimeLeft(float seconds);
    void start();
    void stop();

protected:
    Clock() = default;

private:
    void HandleUpdate(float seconds) override;

    float m_timeLeft;
    bool m_running;
};
