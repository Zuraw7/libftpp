#include "../test_utils.hpp"
#include "../../includes/time/timer.hpp"
#include <chrono>
#include <thread>

namespace {
	void sleepMs(int ms) {
		std::this_thread::sleep_for(std::chrono::milliseconds(ms));
	}
	std::chrono::milliseconds ms(int n) {
		return std::chrono::milliseconds(n);
	}
}

void timer_tests() {
	TEST_SECTION("Timer");

	// freshly started: not elapsed, remaining close to the full duration
	{
		Timer t(ms(100));
		t.start();
		EXPECT(t.isElapsed() == false);
		EXPECT_NEAR(t.remaining().count(), 100, 30);
	}

	// partway through: still not elapsed, remaining shrinks
	{
		Timer t(ms(100));
		t.start();
		sleepMs(60);
		EXPECT(t.isElapsed() == false);
		EXPECT_NEAR(t.remaining().count(), 40, 30);
	}

	// after the duration passes: elapsed, remaining clamped to 0
	{
		Timer t(ms(50));
		t.start();
		sleepMs(80);
		EXPECT(t.isElapsed() == true);
		EXPECT(t.remaining().count() == 0);
	}

	// changeDuration extends the deadline without restarting the countdown
	{
		Timer t(ms(50));
		t.start();
		sleepMs(60);                 // would be elapsed at 50ms
		t.changeDuration(ms(200));   // move deadline out, keep elapsed time
		EXPECT(t.isElapsed() == false);
		EXPECT_NEAR(t.remaining().count(), 140, 40);
	}

	// changeDuration below the elapsed time makes it immediately elapsed
	{
		Timer t(ms(100));
		t.start();
		sleepMs(60);
		t.changeDuration(ms(50));    // 60ms already passed > 50ms
		EXPECT(t.isElapsed() == true);
	}

	// start() restarts the countdown
	{
		Timer t(ms(50));
		t.start();
		sleepMs(80);
		EXPECT(t.isElapsed() == true);
		t.start();                   // rearm
		EXPECT(t.isElapsed() == false);
		EXPECT_NEAR(t.remaining().count(), 50, 30);
	}
}
