#ifndef PERSISTENT_WORKER_HPP
#define PERSISTENT_WORKER_HPP

#include "thread.hpp"
#include <unordered_map>
#include <string>
#include <functional>
#include <mutex>
#include <atomic>

/**
 * @brief A thread that continuously executes a set of named tasks in a loop.
 *
 * The worker thread starts automatically on construction and runs until the
 * object is destroyed. Each iteration copies the task map under a mutex and
 * executes all tasks - so addTask/removeTask take effect on the next iteration.
 *
 * Use threadSafeCount inside tasks for prefixed thread-safe output.
 *
 * @var m_thread   The underlying worker thread.
 * @var m_tasks    Map of named tasks executed each iteration.
 * @var m_mutex    Protects m_tasks during add/remove/copy operations.
 * @var m_running  Flag set to false on destruction to stop the loop.
 */
class PersistentWorker {
public:
    /**
     * @brief Starts the worker thread immediately.
     * @param threadPrefix Name used as threadSafeCount prefix inside tasks.
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
    Thread m_thread;
    std::unordered_map<std::string, std::function<void()>> m_tasks;
    std::mutex m_mutex;
    std::atomic<bool> m_running = true;
};

#endif
