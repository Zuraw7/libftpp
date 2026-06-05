#include "../test_utils.hpp"
#include "../../includes/designPatterns/singleton.hpp"

namespace {
	class Config : public Singleton<Config> {
		friend class Singleton<Config>;
	public:
		Config() = default;          // public ctor for the handle object
		int value = 0;
	private:
		Config(int v) : value(v) {}  // managed instance, created via Singleton (friend)
	};
}

void singleton_tests() {
	TEST_SECTION("Singleton");

	// instance() before instantiate() throws
	{
		Config c;
		EXPECT_THROW(c.instance());
	}

	// instantiate() creates the object; instance() returns it with forwarded args
	{
		Config c;
		c.instantiate(42);
		EXPECT(c.instance()->value == 42);
	}

	// instantiate() twice throws
	{
		Config c;
		c.instantiate(1);
		EXPECT_THROW(c.instantiate(2));
	}

	// instance() returns the same pointer across calls
	{
		Config c;
		c.instantiate(5);
		EXPECT(c.instance() == c.instance());
	}

	// mutations through the instance pointer persist
	{
		Config c;
		c.instantiate(0);
		c.instance()->value = 99;
		EXPECT(c.instance()->value == 99);
	}
}
