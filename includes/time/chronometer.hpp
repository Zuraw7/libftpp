#ifndef CHRONOMETER_HPP
#define CHRONOMETER_HPP

#include <chrono>

/**
 * @brief Stopwatch for measuring elapsed time with pause/resume support.
 *
 * Counts up while running. start() begins (or resumes) timing and stop()
 * pauses it; the elapsed span of every run is accumulated, so repeated
 * start/stop cycles add together. Uses a steady (monotonic) clock, so it is
 * unaffected by changes to the system wall clock.
 *
 * @code
 * Chronometer c;
 * c.start();
 * // ... work ...
 * c.stop();
 * c.start();
 * // ... more work ...
 * c.stop();
 * c.totalElapsed();   // total across both runs
 * @endcode
 */
class Chronometer {
public:
    /// @brief Constructs a stopped stopwatch with zero accumulated time.
    Chronometer();

    /// @brief Begins or resumes timing. No effect if already running.
    void start();

    /// @brief Pauses timing, adding the current run to the accumulated total.
    ///        No effect if not running.
    void stop();

    /// @brief Clears the accumulated time and stops the stopwatch.
    void reset();

    /**
     * @brief Total elapsed time across all runs.
     *
     * Includes the currently running interval if the stopwatch is active.
     * @return Accumulated time in milliseconds (sub-millisecond part truncated).
     */
    std::chrono::milliseconds totalElapsed() const;

    /**
     * @brief Time elapsed since the last start().
     *
     * @return The current run's duration in milliseconds, or 0 if stopped.
     */
    std::chrono::milliseconds currentElapsed() const;

    /// @brief Whether the stopwatch is currently running.
    bool isRunning() const;

private:
    std::chrono::steady_clock::time_point m_startTime;      ///< Start of the current run.
    std::chrono::steady_clock::duration m_accumulatedTime;  ///< Sum of completed runs.
    bool m_running = false;                                 ///< True while timing.
};

#endif
