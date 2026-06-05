#include "test_utils.hpp"
#include "tests.hpp"

int main() {

	data_buffer_tests();

	std::cout << "\n" << g_pass << " passed, " << g_fail << " failed\n";
	return g_fail ? 1 : 0;
}
