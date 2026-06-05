#include "../test_utils.hpp"
#include "../../includes/designPatterns/observable_value.hpp"
#include <string>

void observable_value_tests() {
	TEST_SECTION("ObservableValue");

	// constructor stores the initial value
	{
		ObservableValue<int> v = 10;
		EXPECT(v.get() == 10);
	}

	// set() updates the value and notifies once
	{
		ObservableValue<int> v = 0;
		int calls = 0;
		v.subscribe([&calls] { ++calls; });
		v.set(5);
		EXPECT(v.get() == 5);
		EXPECT(calls == 1);
	}

	// operator= updates the value and notifies once
	{
		ObservableValue<int> v = 0;
		int calls = 0;
		v.subscribe([&calls] { ++calls; });
		v = 7;
		EXPECT(v.get() == 7);
		EXPECT(calls == 1);
	}

	// every mutation fires a notification
	{
		ObservableValue<int> v = 0;
		int calls = 0;
		v.subscribe([&calls] { ++calls; });
		v.set(1);
		v = 2;
		v.set(3);
		EXPECT(calls == 3);
	}

	// multiple subscribers are all invoked, in subscription order
	{
		ObservableValue<int> v = 0;
		std::string order;
		v.subscribe([&order] { order += "A"; });
		v.subscribe([&order] { order += "B"; });
		v = 1;
		EXPECT(order == "AB");
	}

	// reads do not notify
	{
		ObservableValue<int> v = 42;
		int calls = 0;
		v.subscribe([&calls] { ++calls; });
		int viaGet = v.get();
		int viaConv = v;            // implicit conversion
		EXPECT(viaGet == 42);
		EXPECT(viaConv == 42);
		EXPECT(calls == 0);
	}

	// works with a class type
	{
		ObservableValue<std::string> name = std::string("Ada");
		int calls = 0;
		name.subscribe([&calls] { ++calls; });
		name = std::string("Grace");
		EXPECT(name.get() == "Grace");
		EXPECT(calls == 1);
	}
}
