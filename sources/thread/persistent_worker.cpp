#include "../../includes/thread/persistent_worker.hpp"
#include <mutex>

PersistentWorker::PersistentWorker(const std::string &threadPrefix)
    : m_thread(threadPrefix, [this](){
        while(m_running) {
            std::unique_lock<std::mutex> lock(m_mutex);
            auto tasksCopy = m_tasks;
            lock.unlock();

            for (auto &[name, task] : tasksCopy) {
                task();
            }
        }
    }) {
    m_thread.start();
}

PersistentWorker::~PersistentWorker() {
    m_running = false;
    m_thread.stop();
}

void PersistentWorker::addTask(const std::string& name, const std::function<void()>& jobToExecute) {
    std::unique_lock<std::mutex> lock(m_mutex);
    m_tasks[name] = jobToExecute;
}

void PersistentWorker::removeTask(const std::string& name) {
    std::unique_lock<std::mutex> lock(m_mutex);
    m_tasks.erase(name);
}

void PersistentWorker::clearTasks() {
    std::unique_lock<std::mutex> lock(m_mutex);
    m_tasks.clear();
}
