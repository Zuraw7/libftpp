#include "../../includes/thread/thread.hpp"

Thread::Thread(const std::string& name, std::function<void()> funcToExecute)
    : m_name(name), m_func(funcToExecute) {
    
}

void Thread::start() {
    m_thread = std::thread([this]() {
        threadSafeCount.setPrefix("[" + m_name + "] ");
        m_func();
    });
}

void Thread::stop() {
    m_thread.join();
}
