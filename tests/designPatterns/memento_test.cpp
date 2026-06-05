#include "../test_utils.hpp"
#include "../../includes/designPatterns/memento.hpp"

namespace {
	// A saveable object built on the Memento base.
	class Player : public Memento {
	public:
		int health = 0;
		int x = 0;
		int y = 0;

	private:
		void _saveToSnapshot(Snapshot& s) const override {
			s << health << x << y;
		}
		void _loadFromSnapshot(Snapshot& s) override {
			s >> health >> x >> y;
		}
	};
}

void memento_tests() {
	TEST_SECTION("Memento");

	// save then mutate then load restores every field
	{
		Player p;
		p.health = 100;
		p.x = 5;
		p.y = 7;
		auto snap = p.save();

		p.health = 0;
		p.x = 0;
		p.y = 0;
		p.load(snap);
		EXPECT(p.health == 100);
		EXPECT(p.x == 5);
		EXPECT(p.y == 7);
	}

	// a snapshot is independent of later changes to the object
	{
		Player p;
		p.health = 50;
		auto snap = p.save();
		p.health = 999;     // change after snapshot
		p.load(snap);
		EXPECT(p.health == 50);
	}

	// the same snapshot can be loaded more than once
	{
		Player p;
		p.health = 33;
		auto snap = p.save();

		p.health = 1;
		p.load(snap);
		EXPECT(p.health == 33);

		p.health = 2;
		p.load(snap);
		EXPECT(p.health == 33);
	}

	// a snapshot from one instance restores into another
	{
		Player a;
		a.health = 11;
		a.x = 22;
		a.y = 33;
		auto snap = a.save();

		Player b;
		b.load(snap);
		EXPECT(b.health == 11);
		EXPECT(b.x == 22);
		EXPECT(b.y == 33);
	}
}
