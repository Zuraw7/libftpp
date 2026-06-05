#ifndef THREAD_HPP
#define THREAD_HPP

#include "../iostream/thread_safe_iostream.hpp"
#include <functional>
#include <thread>

extern thread_local ThreadSafeIOStream threadSafeCout;

/**
 * @brief Wrapper around std::thread with a name used as a ThreadSafeIOStream prefix.
 *
 * The thread does not start automatically - call start() explicitly after construction.
 * Use stop() to join the thread before the object is destroyed, otherwise std::terminate
 * will be called.
 *
 * Any function passed to the constructor that uses threadSafeCout will automatically
 * have its output prefixed with "[name] ".
 *
 * @code
 * Thread t("worker", []() {
 *     threadSafeCout << "hello\n";  // prints: [worker] hello
 * });
 * t.start();
 * t.stop();
 * @endcode
 */
class Thread {
public:
    /**
     * @brief Sets up thread data. Does not start execution.
     * @param name          Name used as threadSafeCout prefix inside the thread.
     * @param funcToExecute Function to run when start() is called.
     */
    Thread(const std::string& name, std::function<void()> funcToExecute);

    /** @brief Launches the thread, executing the function passed to the constructor. */
    void start();

    /** @brief Joins the thread, blocking until it finishes execution. */
    void stop();

private:
    std::string m_name;            ///< Thread name, used as prefix for threadSafeCout output.
    std::function<void()> m_func;  ///< Function executed when start() is called.
    std::thread m_thread;          ///< Underlying std::thread, created on start().
};

#endif
