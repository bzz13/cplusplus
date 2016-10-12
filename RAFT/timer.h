#pragma once

#include <time.h>
#include <cstdlib>
#include <chrono>

class timer
{
    std::chrono::high_resolution_clock::time_point  m_start_waiting_clock;
    std::chrono::milliseconds                       m_waiting_period;
public:
    timer()
    {
        srand(time(nullptr));
    }

    void start()
    {
        m_start_waiting_clock = std::chrono::high_resolution_clock::now();
        m_waiting_period = std::chrono::milliseconds(rand() % 500 + 500);
    }

    void reset()
    {
        start();
    }

    bool isExpired()
    {
        auto clock_now = std::chrono::high_resolution_clock::now();
        auto duration = clock_now - m_start_waiting_clock;
        auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
        return duration >= m_waiting_period;
    }

};