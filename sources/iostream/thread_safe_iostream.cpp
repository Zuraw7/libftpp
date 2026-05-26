#include "../../includes/iostream/thread_safe_iostream.hpp"
#include <ostream>

std::mutex ThreadSafeIOStream::m_mutex;
thread_local ThreadSafeIOStream threadSafeCount;

ThreadSafeIOStream::~ThreadSafeIOStream() {
    if (!m_buffer.str().empty()) {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::cout << m_prefix << m_buffer.str() << std::flush;
    }
}

ThreadSafeIOStream &ThreadSafeIOStream::operator<<(std::ostream& (*f)(std::ostream&)) {
    std::lock_guard<std::mutex> lock(m_mutex);
    std::cout << m_prefix << m_buffer.str();
    f(std::cout);
    m_buffer.str("");

    return *this;
}

void ThreadSafeIOStream::setPrefix(const std::string &prefix) {
    m_prefix = prefix;
}
