#ifndef THREAD_SAFE_QUEUE_HPP
#define THREAD_SAFE_QUEUE_HPP

#include <deque>
#include <mutex>
#include <stdexcept>

template<typename TType>
class ThreadSafeQueue {
public:
    void push_back(const TType &newElement){
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queue.push_back(newElement);
    }

    void push_front(const TType &newElement) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queue.push_front(newElement);
    }

    TType pop_back() {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_queue.empty())
            throw std::runtime_error("ThreadSafeQueue: Can't pop_back, queue is empty");

        TType element = m_queue.back();
        m_queue.pop_back();
        return element;
    }

    TType pop_front() {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_queue.empty())
            throw std::runtime_error("ThreadSafeQueue: Can't pop_front, queue is empty");

        TType element = m_queue.front();
        m_queue.pop_front();
        return element;
    }

private:
    std::deque<TType> m_queue;
    std::mutex m_mutex;
};

#endif
