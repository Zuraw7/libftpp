#ifndef THREAD_SAFE_QUEUE_HPP
#define THREAD_SAFE_QUEUE_HPP

#include <deque>
#include <mutex>
#include <stdexcept>

/**
 * @brief Templated thread-safe queue backed by std::deque.
 *
 * Multiple threads can push and pop concurrently without data races.
 * All operations are protected by a single mutex.
 *
 * @tparam TType Type of elements stored in the queue.
 *
 * @var m_queue Internal deque holding the elements.
 * @var m_mutex Protects all accesses to m_queue.
 */
template<typename TType>
class ThreadSafeQueue {
public:
    /** @brief Adds an element at the end of the queue. */
    void push_back(const TType &newElement){
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queue.push_back(newElement);
    }

    /** @brief Adds an element at the front of the queue. */
    void push_front(const TType &newElement) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queue.push_front(newElement);
    }

    /**
     * @brief Extracts and returns the last element.
     * @throws std::runtime_error if the queue is empty.
     */
    TType pop_back() {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_queue.empty())
            throw std::runtime_error("ThreadSafeQueue: Can't pop_back, queue is empty");

        TType element = m_queue.back();
        m_queue.pop_back();
        return element;
    }

    /**
     * @brief Extracts and returns the first element.
     * @throws std::runtime_error if the queue is empty.
     */
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
