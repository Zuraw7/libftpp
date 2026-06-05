#include "../test_utils.hpp"
#include "../../includes/thread/thread.hpp"
#include <atomic>

void thread_tests() {
	TEST_SECTION("Thread");

	// start() runs the function; stop() joins so the effect is visible
	{
		std::atomic<bool> ran{false};
		Thread t("t", [&ran] { ran = true; });
		t.start();
		t.stop();
		EXPECT(ran.load());
	}

	// the function runs exactly once
	{
		std::atomic<int> count{0};
		Thread t("t", [&count] { ++count; });
		t.start();
		t.stop();
		EXPECT(count.load() == 1);
	}

	// work completed in the thread is fully done after stop() returns
	{
		std::atomic<int> sum{0};
		Thread t("t", [&sum] {
			for (int i = 1; i <= 100; ++i)
				sum += i;
		});
		t.start();
		t.stop();
		EXPECT(sum.load() == 5050);
	}

	// independent threads run their own functions
	{
		std::atomic<int> a{0};
		std::atomic<int> b{0};
		Thread t1("a", [&a] { a = 1; });
		Thread t2("b", [&b] { b = 2; });
		t1.start();
		t2.start();
		t1.stop();
		t2.stop();
		EXPECT(a.load() == 1);
		EXPECT(b.load() == 2);
	}
}
