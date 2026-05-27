#ifndef WORKER_POOL_HPP
#define WORKER_POOL_HPP

#include "thread.hpp"
#include <condition_variable>
#include <functional>
#include <mutex>
#include <vector>
#include <queue>
#include <atomic>

/**
 * @brief Manages a pool of worker threads that execute jobs concurrently.
 *
 * Threads are created on construction and run perpetually until the pool
 * is destroyed. On destruction, all pending jobs in the queue are finished
 * before threads are joined.
 *
 * Jobs can be submitted in two ways - see addJob() overloads.
 * Inside jobs, use threadSafeCount for thread-safe output with the worker's
 * name as prefix (e.g. "[worker_0] message").
 *
 * @var m_threads  Worker threads created on construction.
 * @var m_jobs     Queue of pending jobs, protected by m_mutex.
 * @var m_running  Flag set to false on destruction to signal threads to stop.
 * @var m_mutex    Protects m_jobs and coordinates with m_condVar.
 * @var m_condVar  Wakes idle threads when a new job is added or pool is stopping.
 *
 * @example
 * @code
 * WorkerPool pool(4);
 *
 * // lambda
 * pool.addJob([]() {
 *     threadSafeCount << "hello from worker\n";
 * });
 *
 * // IJobs
 * class MyJob : public WorkerPool::IJobs {
 * public:
 *     void execute() override {
 *         threadSafeCount << "hello from IJobs\n";
 *     }
 * };
 * MyJob job;
 * pool.addJob(&job);
 * @endcode
 */
class WorkerPool {
public:
    /** @brief Creates the pool and starts threadNb worker threads. */
    WorkerPool(int threadNb);

    /** @brief Finishes all pending jobs, then joins all threads (graceful shutdown). */
    ~WorkerPool();

    /**
     * @brief Interface for jobs submitted via addJob(IJobs*).
     *
     * Inherit from this class and implement execute() to define job behavior.
     * Make sure the IJobs instance outlives the WorkerPool - the pool holds
     * a raw pointer and will call execute() from a worker thread.
     */
    class IJobs {
        public:
            virtual void execute() = 0;
            virtual ~IJobs() = default;
    };

    /** @brief Submits a job as a callable (lambda, function, functor). */
    void addJob(const std::function<void()>& jobToExecute);

    /** @brief Submits a job via the IJobs interface - calls job->execute() on a worker thread. */
    void addJob(IJobs* job);

private:
    std::vector<Thread> m_threads;
    std::queue<std::function<void()>> m_jobs;
    std::atomic<bool> m_running = true;
    std::mutex m_mutex;
    std::condition_variable m_condVar;
};

#endif
