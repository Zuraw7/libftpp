#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono>

/**
 * @brief Countdown timer that reports when a fixed duration has elapsed.
 *
 * Set a duration, arm it with start(), then poll isElapsed() to learn whether
 * that much time has passed. Uses a steady (monotonic) clock, so it measures
 * true elapsed time and is unaffected by changes to the system wall clock.
 *
 * @code
 * Timer t(std::chrono::milliseconds(100));
 * t.start();
 * while (!t.isElapsed()) {
 *     // ... do other work ...
 * }
 * @endcode
 */
class Timer {
public:
	/// @param duration How long after start() the timer is considered elapsed.
	Timer(std::chrono::milliseconds duration);

	/// @brief (Re)arms the timer, restarting the countdown from now.
	void start();

	/// @brief Whether the duration has passed since the last start().
	bool isElapsed() const;

	/**
	 * @brief Time left before the timer elapses.
	 * @return Remaining time in milliseconds, or 0 if already elapsed.
	 */
	std::chrono::milliseconds remaining() const;

	/**
	 * @brief Changes the duration without restarting the countdown.
	 *
	 * The elapsed time so far is kept; only the deadline moves. Shrinking the
	 * duration below the elapsed time makes the timer immediately elapsed.
	 *
	 * @param duration The new total duration measured from the last start().
	 */
	void changeDuration(std::chrono::milliseconds duration);

private:
	std::chrono::steady_clock::time_point m_start;  ///< When the countdown was armed.
	std::chrono::milliseconds m_duration;           ///< Length of the countdown.
};

#endif
