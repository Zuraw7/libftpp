#include "../test_utils.hpp"
#include "../../includes/thread/worker_pool.hpp"
#include <atomic>

namespace {
	struct CountingJob : public WorkerPool::IJobs {
		std::atomic<int>& counter;
		CountingJob(std::atomic<int>& c) : counter(c) {}
		void execute() override { ++counter; }
	};
}

void worker_pool_tests() {
	TEST_SECTION("WorkerPool");

	// every submitted lambda job runs before the pool finishes shutting down
	{
		std::atomic<int> count{0};
		{
			WorkerPool pool(4);
			for (int i = 0; i < 100; ++i)
				pool.addJob([&count] { ++count; });
		}  // destructor drains pending jobs, then joins
		EXPECT(count.load() == 100);
	}

	// IJobs* jobs are executed via execute()
	{
		std::atomic<int> count{0};
		CountingJob job(count);   // must outlive the pool
		{
			WorkerPool pool(2);
			pool.addJob(&job);
			pool.addJob(&job);
			pool.addJob(&job);
		}
		EXPECT(count.load() == 3);
	}

	// many concurrent jobs: results are correct and none are lost
	{
		std::atomic<int> sum{0};
		{
			WorkerPool pool(8);
			for (int i = 1; i <= 1000; ++i)
				pool.addJob([&sum, i] { sum += i; });
		}
		EXPECT(sum.load() == 500500);   // 1 + 2 + ... + 1000
	}

	// a pool with no jobs shuts down cleanly
	{
		WorkerPool pool(3);
		EXPECT(true);
	}
}
