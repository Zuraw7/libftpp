#include "../../includes/thread/worker_pool.hpp"
#include <string>

WorkerPool::WorkerPool(int threadNb) {
    m_threads.reserve(threadNb);
    for (int i = 0; i < threadNb; i++) {
        m_threads.emplace_back("worker_" + std::to_string(i), [this](){
            while(m_running || !m_jobs.empty()) {
                std::unique_lock<std::mutex> lock(m_mutex);
                m_condVar.wait(lock, [this](){ return !m_jobs.empty() || !m_running; });
                if (!m_running && m_jobs.empty())
                    break;

                auto job = m_jobs.front();
                m_jobs.pop();
                lock.unlock();
                job();
            }
        });
        m_threads.back().start();
    }
}

WorkerPool::~WorkerPool() {
    m_running = false;
    m_condVar.notify_all();
    for (auto &thread: m_threads) {
        thread.stop();
    }
}

void WorkerPool::addJob(const std::function<void()>&jobToExecute) {
    std::unique_lock<std::mutex> lock(m_mutex);
    m_jobs.push(jobToExecute);
    lock.unlock();
    m_condVar.notify_one();
}

void WorkerPool::addJob(IJobs *job) {
    addJob([job]() { job->execute(); });
}
