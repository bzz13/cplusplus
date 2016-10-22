#pragma once

#include <queue>
#include <iostream>
#include <mutex>
#include <utility>

template<typename ItemT>
class thread_safe_queue
{
    typedef std::unique_lock<std::mutex> lock;
    typedef std::pair<bool, ItemT> pair;

    std::queue<ItemT>   m_queue;
    std::mutex          m_mtx;
public:
    pair try_pop()
    {
        lock lk(m_mtx);
        if (!m_queue.empty())
        {
            pair resutl(true, m_queue.front());
            m_queue.pop();
            return resutl;
        }
        return {false, ItemT()};
    }

    void push(ItemT item)
    {
        lock lk(m_mtx);
        m_queue.push(item);
    }

    int size()
    {
        lock lk(m_mtx);
        return m_queue.size();
    }
};