#include "../../includes/time/chronometer.hpp"

Chronometer::Chronometer() {
    reset();
}

void Chronometer::start() {
    if (m_running)
        return;
    m_startTime = std::chrono::steady_clock::now();
    m_running = true;
}

void Chronometer::stop() {
    if (!m_running)
        return;
    m_accumulatedTime += std::chrono::steady_clock::now() - m_startTime;
    m_running = false;
}

void Chronometer::reset() {
    m_accumulatedTime = std::chrono::steady_clock::duration::zero();
    m_running = false;
}

std::chrono::milliseconds Chronometer::totalElapsed() const {
    auto total = m_accumulatedTime;
    if (m_running)
        total += std::chrono::steady_clock::now() - m_startTime;
    return std::chrono::duration_cast<std::chrono::milliseconds>(total);
}

std::chrono::milliseconds Chronometer::currentElapsed() const {
    if (!m_running)
        return std::chrono::milliseconds {0};
    auto elapsed = std::chrono::steady_clock::now() - m_startTime;
    return std::chrono::duration_cast<std::chrono::milliseconds>(elapsed);
}

bool Chronometer::isRunning() const {
    return m_running;
}
