#ifndef THREAD_SAFE_IOSTREAM_HPP
#define THREAD_SAFE_IOSTREAM_HPP

#include <iostream>
#include <mutex>
#include <sstream>
#include <string>

/**
 * @brief Thread-safe wrapper around std::cout/std::cin with per-line prefix support.
 *
 * Each thread owns its own instance via thread_local (see threadSafeCout).
 * Output is buffered per-instance and flushed atomically on '\n' or std::endl,
 * so lines from concurrent threads never interleave.
 *
 * @var m_mutex  Shared (static) across all instances - protects std::cout/std::cin.
 * @var m_prefix Per-instance prefix prepended to each flushed line.
 * @var m_buffer Per-instance output buffer, flushed on '\n' or std::endl.
 *
 * @example
 * @code
 * threadSafeCout.setPrefix("[thread1] ");
 * threadSafeCout << "hello\n";    // prints: [thread1] hello
 * threadSafeCout << "age: " << 42 << std::endl;
 * @endcode
 */
class ThreadSafeIOStream;

/// Thread-local instance - one per thread, usable without manual instantiation.
extern thread_local ThreadSafeIOStream threadSafeCout;

class ThreadSafeIOStream {
public:
    /** @brief Handles std::endl and std::flush - flushes buffer to std::cout with prefix. */
    ThreadSafeIOStream& operator<<(std::ostream& (*f)(std::ostream&));

    /**
     * @brief Appends value to internal buffer. Flushes each complete line (ending with '\n')
     *        to std::cout with the prefix prepended. Incomplete lines stay buffered.
     */
    template<typename T>
    ThreadSafeIOStream& operator<<(const T &value) {
        m_buffer << value;
        std::string s = m_buffer.str();
        std::string output;
        size_t pos;

        while ((pos = s.find('\n')) != std::string::npos) {
            output += m_prefix + s.substr(0, pos + 1);
            s = s.substr(pos + 1);
        }

        if (!output.empty()) {
            std::lock_guard<std::mutex> lock(m_mutex);
            std::cout << output;
        }

        m_buffer.str("");
        m_buffer << s;
        return *this;
    }

    /** @brief Reads a value from std::cin under the shared mutex. */
    template<typename T>
    ThreadSafeIOStream& operator>>(T &value) {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::cin >> value;
        return *this;
    }

    /** @brief Sets the prefix printed before each flushed line. */
    void setPrefix(const std::string& prefix);

    /**
     * @brief Prints prefix+question, then reads answer into dest - all under the mutex
     *        so no other thread can interleave between the question and the input.
     */
    template<typename T>
    void prompt(const std::string& question, T& dest) {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::cout << m_prefix << question;
        std::cin >> dest;
    }

    /** @brief Flushes any remaining buffered output when the thread exits. */
    ~ThreadSafeIOStream();

private:
    static std::mutex m_mutex;
    std::string m_prefix;
    std::ostringstream m_buffer;
};

#endif
