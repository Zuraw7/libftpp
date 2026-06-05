#include "../test_utils.hpp"
#include "../../includes/time/chronometer.hpp"
#include <chrono>
#include <thread>

namespace {
	void sleepMs(int ms) {
		std::this_thread::sleep_for(std::chrono::milliseconds(ms));
	}
}

void chronometer_tests() {
	TEST_SECTION("Chronometer");

	// a fresh chronometer is stopped with zero elapsed
	{
		Chronometer c;
		EXPECT(c.isRunning() == false);
		EXPECT(c.totalElapsed().count() == 0);
		EXPECT(c.currentElapsed().count() == 0);
	}

	// start sets it running; currentElapsed grows
	{
		Chronometer c;
		c.start();
		EXPECT(c.isRunning() == true);
		sleepMs(100);
		EXPECT_NEAR(c.currentElapsed().count(), 100, 40);
	}

	// stop freezes the total and clears the running flag
	{
		Chronometer c;
		c.start();
		sleepMs(100);
		c.stop();
		EXPECT(c.isRunning() == false);
		EXPECT_NEAR(c.totalElapsed().count(), 100, 40);
	}

	// repeated start/stop cycles accumulate
	{
		Chronometer c;
		c.start();
		sleepMs(100);
		c.stop();
		c.start();
		sleepMs(100);
		c.stop();
		EXPECT_NEAR(c.totalElapsed().count(), 200, 60);
	}

	// totalElapsed includes the in-progress interval while running
	{
		Chronometer c;
		c.start();
		sleepMs(100);
		EXPECT_NEAR(c.totalElapsed().count(), 100, 40);   // not stopped
	}

	// currentElapsed is zero once stopped
	{
		Chronometer c;
		c.start();
		sleepMs(50);
		c.stop();
		EXPECT(c.currentElapsed().count() == 0);
	}

	// reset clears accumulated time and stops
	{
		Chronometer c;
		c.start();
		sleepMs(50);
		c.stop();
		c.reset();
		EXPECT(c.isRunning() == false);
		EXPECT(c.totalElapsed().count() == 0);
	}
}
