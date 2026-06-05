#include "test_utils.hpp"
#include "tests.hpp"

int main() {

	std::cout << "\n=== Data structures ===\n";
	data_buffer_tests();
	pool_tests();

	std::cout << "\n=== Design patterns ===\n";
	memento_tests();
	observable_value_tests();
	observer_tests();
	singleton_tests();
	state_machine_tests();

	std::cout << "\n=== IOStream ===\n";
	thread_safe_iostream_tests();

	std::cout << "\n=== Mathematics ===\n";
	ivector2_tests();
	ivector3_tests();
	perlin_noise_2D_tests();
	random_2D_coordinate_generator_tests();

	std::cout << "\n=== Networking ===\n";
	message_tests();
	client_tests();
	server_tests();

	std::cout << "\n=== Threading ===\n";
	thread_tests();
	persistent_worker_tests();
	thread_safe_queue_tests();
	worker_pool_tests();

	std::cout << "\n" << g_pass << " passed, " << g_fail << " failed\n";
	return g_fail ? 1 : 0;
}
