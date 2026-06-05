#include "../test_utils.hpp"
#include "../../includes/designPatterns/observer.hpp"
#include <string>

namespace {
	enum class Event { Start, Stop };
}

void observer_tests() {
	TEST_SECTION("Observer");

	// notify invokes the subscribed callback
	{
		Observer<std::string> obs;
		int calls = 0;
		obs.subscribe("click", [&calls] { ++calls; });
		obs.notify("click");
		EXPECT(calls == 1);
	}

	// only subscribers of the notified event fire
	{
		Observer<std::string> obs;
		int clicks = 0;
		int hovers = 0;
		obs.subscribe("click", [&clicks] { ++clicks; });
		obs.subscribe("hover", [&hovers] { ++hovers; });
		obs.notify("click");
		EXPECT(clicks == 1);
		EXPECT(hovers == 0);
	}

	// multiple subscribers to one event fire in subscription order
	{
		Observer<std::string> obs;
		std::string order;
		obs.subscribe("e", [&order] { order += "A"; });
		obs.subscribe("e", [&order] { order += "B"; });
		obs.subscribe("e", [&order] { order += "C"; });
		obs.notify("e");
		EXPECT(order == "ABC");
	}

	// notifying an event with no subscribers is a harmless no-op
	{
		Observer<std::string> obs;
		obs.notify("nobody");
		EXPECT(true);
	}

	// notifying repeatedly fires every time
	{
		Observer<std::string> obs;
		int calls = 0;
		obs.subscribe("tick", [&calls] { ++calls; });
		obs.notify("tick");
		obs.notify("tick");
		obs.notify("tick");
		EXPECT(calls == 3);
	}

	// works with an enum event key
	{
		Observer<Event> obs;
		int started = 0;
		int stopped = 0;
		obs.subscribe(Event::Start, [&started] { ++started; });
		obs.subscribe(Event::Stop, [&stopped] { ++stopped; });
		obs.notify(Event::Start);
		EXPECT(started == 1);
		EXPECT(stopped == 0);
	}
}
