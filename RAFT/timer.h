#pragma once

#include <time.h>
#include <cstdlib>
#include <chrono>

class timer
{
    std::chrono::high_resolution_clock::time_point  m_start_waiting_clock;
    std::chrono::milliseconds                       m_waiting_period;
    bool                                            m_started = false;
public:
    timer()
    {
        srand(time(nullptr));
    }

    void start()
    {
        m_started = true;
        m_start_waiting_clock = std::chrono::high_resolution_clock::now();
        // m_waiting_period = std::chrono::milliseconds(rand() % 5000 + 1000);
        m_waiting_period = std::chrono::milliseconds(5000);
    }

    void reset()
    {
        m_started = true;
        m_start_waiting_clock = std::chrono::high_resolution_clock::now();
        // m_waiting_period = std::chrono::milliseconds(rand() % 500 + 750);
        m_waiting_period = std::chrono::milliseconds(5000);
    }

    void clear()
    {
        m_started = true;
    }

    bool isExpired()
    {
        if (!m_started) return false;
        auto clock_now = std::chrono::high_resolution_clock::now();
        auto duration = clock_now - m_start_waiting_clock;
        return duration >= m_waiting_period;
    }

    int getSleepTimeoutMs()
    {
        // return rand() % 500;
        return 1000;
    }
};