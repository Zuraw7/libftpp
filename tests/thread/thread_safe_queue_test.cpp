#include "../test_utils.hpp"
#include "../../includes/thread/thread_safe_queue.hpp"
#include <thread>
#include <vector>

void thread_safe_queue_tests() {
	TEST_SECTION("ThreadSafeQueue");

	// push_back then pop_front is FIFO
	{
		ThreadSafeQueue<int> q;
		q.push_back(1);
		q.push_back(2);
		q.push_back(3);
		EXPECT(q.pop_front() == 1);
		EXPECT(q.pop_front() == 2);
		EXPECT(q.pop_front() == 3);
	}

	// push_back then pop_back returns the most recent element
	{
		ThreadSafeQueue<int> q;
		q.push_back(1);
		q.push_back(2);
		EXPECT(q.pop_back() == 2);
		EXPECT(q.pop_back() == 1);
	}

	// push_front inserts at the head
	{
		ThreadSafeQueue<int> q;
		q.push_front(1);
		q.push_front(2);   // queue is now [2, 1]
		EXPECT(q.pop_front() == 2);
		EXPECT(q.pop_front() == 1);
	}

	// popping an empty queue throws
	{
		ThreadSafeQueue<int> q;
		EXPECT_THROW(q.pop_front());
		EXPECT_THROW(q.pop_back());
	}

	// throwing leaves the queue usable afterwards
	{
		ThreadSafeQueue<int> q;
		EXPECT_THROW(q.pop_front());
		q.push_back(42);
		EXPECT(q.pop_front() == 42);
	}

	// concurrent producers: every pushed element is retrievable, none lost
	{
		ThreadSafeQueue<int> q;
		const int threads = 4;
		const int perThread = 1000;

		std::vector<std::thread> producers;
		for (int t = 0; t < threads; ++t)
			producers.emplace_back([&q] {
				for (int i = 0; i < perThread; ++i)
					q.push_back(i);
			});
		for (auto& p : producers)
			p.join();

		int count = 0;
		try {
			while (true) {
				q.pop_front();
				++count;
			}
		} catch (...) {
			// drained
		}
		EXPECT(count == threads * perThread);
	}
}
