#include "../test_utils.hpp"
#include "../../includes/thread/persistent_worker.hpp"
#include <atomic>
#include <chrono>
#include <thread>

namespace {
	void sleepMs(int ms) {
		std::this_thread::sleep_for(std::chrono::milliseconds(ms));
	}
}

void persistent_worker_tests() {
	TEST_SECTION("PersistentWorker");

	// a task runs repeatedly while registered
	{
		std::atomic<int> counter{0};
		PersistentWorker w("[W] ");
		w.addTask("inc", [&counter] { ++counter; });
		sleepMs(50);
		EXPECT(counter.load() > 0);
	}

	// removeTask stops further execution
	{
		std::atomic<int> counter{0};
		PersistentWorker w("[W] ");
		w.addTask("inc", [&counter] { ++counter; });
		sleepMs(40);
		EXPECT(counter.load() > 0);

		w.removeTask("inc");
		sleepMs(20);                 // let any in-flight iteration settle
		int c1 = counter.load();
		sleepMs(20);
		int c2 = counter.load();
		EXPECT(c1 == c2);            // no more increments
	}

	// multiple tasks all run
	{
		std::atomic<int> a{0};
		std::atomic<int> b{0};
		PersistentWorker w("[W] ");
		w.addTask("a", [&a] { ++a; });
		w.addTask("b", [&b] { ++b; });
		sleepMs(50);
		EXPECT(a.load() > 0);
		EXPECT(b.load() > 0);
	}

	// clearTasks stops everything
	{
		std::atomic<int> counter{0};
		PersistentWorker w("[W] ");
		w.addTask("x", [&counter] { ++counter; });
		sleepMs(30);
		EXPECT(counter.load() > 0);

		w.clearTasks();
		sleepMs(20);
		int c1 = counter.load();
		sleepMs(20);
		int c2 = counter.load();
		EXPECT(c1 == c2);
	}

	// adding a task with an existing name overwrites it
	{
		std::atomic<int> oldRuns{0};
		std::atomic<int> newRuns{0};
		PersistentWorker w("[W] ");
		w.addTask("t", [&oldRuns] { ++oldRuns; });
		sleepMs(20);
		w.addTask("t", [&newRuns] { ++newRuns; });  // overwrite
		sleepMs(10);                                 // let the switch take effect
		int oldSnap = oldRuns.load();
		sleepMs(40);
		EXPECT(newRuns.load() > 0);                  // new task runs
		EXPECT(oldRuns.load() - oldSnap <= 1);       // old task no longer runs (allow 1 in-flight)
	}
}
