#include "../../includes/time/timer.hpp"
#include <chrono>

Timer::Timer(std::chrono::milliseconds duration) : m_duration(duration) { }

void Timer::start() {
	m_start = std::chrono::steady_clock::now();
}

bool Timer::isElapsed() const {
	auto currTime = std::chrono::steady_clock::now();
	return currTime - m_start >= m_duration;
}

std::chrono::milliseconds Timer::remaining() const {
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - m_start);
	auto timeLeft = m_duration - elapsed;
	return timeLeft.count() > 0 ? timeLeft : std::chrono::milliseconds{0};
}

void Timer::changeDuration(std::chrono::milliseconds duration) {
	m_duration = duration;
}
