#include "test_utils.hpp"
#include "tests.hpp"

int main() {

	data_buffer_tests();
	pool_tests();
	memento_tests();
	observable_value_tests();
	observer_tests();
	singleton_tests();
	state_machine_tests();
	thread_safe_iostream_tests();

	std::cout << "\n" << g_pass << " passed, " << g_fail << " failed\n";
	return g_fail ? 1 : 0;
}
