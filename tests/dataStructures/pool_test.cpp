#include "../test_utils.hpp"
#include "../../includes/dataStructures/pool.hpp"

namespace {
	// Tracks live instances so we can verify construction/destruction.
	struct Tracked {
		static int alive;
		int value;
		Tracked(int v = 0) : value(v) { ++alive; }
		~Tracked() { --alive; }
	};
	int Tracked::alive = 0;
}

void pool_tests() {
	TEST_SECTION("Pool");

	// acquire on an empty (unsized) pool throws
	{
		Pool<int> pool;
		EXPECT_THROW(pool.acquire(1));
	}

	// resize then acquire constructs in place and operator-> reads it back
	{
		Pool<int> pool;
		pool.resize(2);
		auto a = pool.acquire(42);
		EXPECT(*a.operator->() == 42);
	}

	// constructor arguments are forwarded
	{
		Pool<Tracked> pool;
		pool.resize(1);
		auto obj = pool.acquire(7);
		EXPECT(obj->value == 7);
	}

	// pool reports full when all slots are taken
	{
		Pool<int> pool;
		pool.resize(1);
		auto only = pool.acquire(1);
		EXPECT(*only.operator->() == 1);
		EXPECT_THROW(pool.acquire(2));
	}

	// a slot is returned to the pool when its Object goes out of scope
	{
		Pool<int> pool;
		pool.resize(1);
		{
			auto first = pool.acquire(10);
			EXPECT(*first.operator->() == 10);
		}
		// slot freed -> acquire succeeds again
		auto second = pool.acquire(20);
		EXPECT(*second.operator->() == 20);
	}

	// construction and destruction are balanced via the RAII handle
	{
		Tracked::alive = 0;
		Pool<Tracked> pool;
		pool.resize(2);
		{
			auto a = pool.acquire(1);
			EXPECT(Tracked::alive == 1);
			auto b = pool.acquire(2);
			EXPECT(Tracked::alive == 2);
			EXPECT(a->value == 1 && b->value == 2);
		}
		EXPECT(Tracked::alive == 0);  // both destroyed on scope exit
	}

	// shrinking is not allowed
	{
		Pool<int> pool;
		pool.resize(3);
		EXPECT_THROW(pool.resize(1));
	}

	// resizing to the current size is a no-op (no throw)
	{
		Pool<int> pool;
		pool.resize(3);
		pool.resize(3);
		EXPECT(true);
	}
}
