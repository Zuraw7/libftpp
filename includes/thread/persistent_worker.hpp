#ifndef PERSISTENT_WORKER_HPP
#define PERSISTENT_WORKER_HPP

#include "thread.hpp"
#include <atomic>
#include <functional>
#include <mutex>
#include <string>
#include <unordered_map>

/**
 * @brief A thread that continuously executes a set of named tasks in a loop.
 *
 * The worker thread starts automatically on construction and runs until the
 * object is destroyed. Each iteration copies the task map under a mutex and
 * executes all tasks - so addTask/removeTask take effect on the next iteration.
 *
 * Use threadSafeCout inside tasks for prefixed thread-safe output.
 */
class PersistentWorker {
public:
    /**
     * @brief Starts the worker thread immediately.
     * @param threadPrefix Name used as threadSafeCout prefix inside tasks.
     */
    PersistentWorker(const std::string& threadPrefix);
    PersistentWorker() = delete;

    /** @brief Stops the worker thread and waits for it to finish. */
    ~PersistentWorker();

    /**
     * @brief Adds a task to the pool. If a task with the same name exists, it is overwritten.
     * @param name Unique identifier for the task.
     * @param jobToExecute Function executed each iteration.
     */
    void addTask(const std::string& name, const std::function<void()>& jobToExecute);

    /**
     * @brief Removes a task by name. Does nothing if the name does not exist.
     * @param name Task to remove.
     */
    void removeTask(const std::string& name);

    /** @brief Removes all tasks. The worker thread keeps running with an empty task list. */
    void clearTasks();

private:
    Thread m_thread;  ///< The underlying worker thread.
    std::unordered_map<std::string, std::function<void()>> m_tasks;  ///< Named tasks executed each iteration.
    std::mutex m_mutex;                 ///< Protects m_tasks during add/remove/copy operations.
    std::atomic<bool> m_running = true; ///< Set to false on destruction to stop the loop.
};

#endif
