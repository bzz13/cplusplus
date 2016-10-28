#pragma once

#include <time.h>
#include <cstdlib>
#include <chrono>

class timer
{
    std::chrono::high_resolution_clock::time_point  m_start_waiting_clock;
    std::chrono::milliseconds                       m_waiting_period;
    bool                                            m_started = false;
    int                                             m_inital_delay;
    int                                             m_inital_delay_rand;
    int                                             m_reset_delay;
    int                                             m_reset_delay_rand;
public:
    timer(int inital_delay, int inital_delay_rand, int reset_delay, int reset_delay_rand)
        : m_inital_delay(inital_delay),
          m_inital_delay_rand(inital_delay_rand),
          m_reset_delay(reset_delay),
          m_reset_delay_rand(reset_delay_rand)
    {
        srand(time(nullptr));
    }

    void start()
    {
        m_started = true;
        m_start_waiting_clock = std::chrono::high_resolution_clock::now();
        m_waiting_period = std::chrono::milliseconds(rand() % m_inital_delay_rand + m_inital_delay);
    }

    void reset()
    {
        m_started = true;
        m_start_waiting_clock = std::chrono::high_resolution_clock::now();
        m_waiting_period = std::chrono::milliseconds(rand() % m_reset_delay_rand + m_reset_delay);
    }

    void clear()
    {
        m_started = false;
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
        return rand() % 500;
    }
};