#include "Clock.hpp"

Clock::Ptr Clock::Create()
{
    Ptr clock(new Clock());
    clock->SetText("00:00");
    clock->m_timeLeft = 0.f;
    clock->m_running = false;
    return clock;
}

void Clock::start()
{
    m_running = true;
}

void Clock::stop()
{
    m_running = false;
}

std::string timeToString(float time)
{
    const int totalSeconds = static_cast<int>(std::ceil(time));
    int minutes = totalSeconds/60;
    int seconds = totalSeconds%60;

    std::string toReturn = std::to_string(minutes) + ":";
    if (seconds<10) toReturn += "0" + std::to_string(seconds);
    else toReturn += std::to_string(seconds);

    return toReturn;
}

void Clock::setTimeLeft(float seconds)
{
    m_timeLeft = seconds;
    SetText(timeToString(m_timeLeft));
}

void Clock::HandleUpdate(float seconds)
{
    sfg::Label::HandleUpdate(seconds);
    if (!m_running)
        return;
    m_timeLeft -= seconds;
    SetText(timeToString(m_timeLeft));
}

