#include "../test_utils.hpp"
#include "../../includes/designPatterns/state_machine.hpp"
#include <string>

namespace {
	enum class S { Idle, Running, Done };
}

void state_machine_tests() {
	TEST_SECTION("StateMachine");

	// transitionTo an unknown state throws
	{
		StateMachine<S> sm;
		sm.addState(S::Idle);
		EXPECT_THROW(sm.transitionTo(S::Running));  // Running was never added
	}

	// transitionTo a known state with no defined transition throws
	{
		StateMachine<S> sm;
		sm.addState(S::Idle);
		sm.addState(S::Running);
		EXPECT_THROW(sm.transitionTo(S::Running));  // no Idle->Running transition
	}

	// a valid transition runs its callback
	{
		StateMachine<S> sm;
		sm.addState(S::Idle);
		sm.addState(S::Running);
		bool fired = false;
		sm.addTransition(S::Idle, S::Running, [&fired] { fired = true; });
		sm.transitionTo(S::Running);
		EXPECT(fired);
	}

	// after a transition, update() runs the new current state's action
	{
		StateMachine<S> sm;
		sm.addState(S::Idle);
		sm.addState(S::Running);
		sm.addTransition(S::Idle, S::Running, [] {});
		int marker = 0;
		sm.addAction(S::Running, [&marker] { marker = 1; });
		sm.transitionTo(S::Running);
		sm.update();
		EXPECT(marker == 1);
	}

	// update() runs the initial state's action (first added state is current)
	{
		StateMachine<S> sm;
		int ticks = 0;
		sm.addState(S::Idle);
		sm.addAction(S::Idle, [&ticks] { ++ticks; });
		sm.update();
		sm.update();
		EXPECT(ticks == 2);
	}

	// update() with no action for the current state throws
	{
		StateMachine<S> sm;
		sm.addState(S::Idle);
		EXPECT_THROW(sm.update());
	}

	// a full path: Idle -> Running -> Done with per-state actions
	{
		StateMachine<S> sm;
		sm.addState(S::Idle);
		sm.addState(S::Running);
		sm.addState(S::Done);
		sm.addTransition(S::Idle, S::Running, [] {});
		sm.addTransition(S::Running, S::Done, [] {});

		std::string log;
		sm.addAction(S::Running, [&log] { log += "R"; });
		sm.addAction(S::Done, [&log] { log += "D"; });

		sm.transitionTo(S::Running);
		sm.update();                 // R
		sm.transitionTo(S::Done);
		sm.update();                 // D
		EXPECT(log == "RD");
	}
}
