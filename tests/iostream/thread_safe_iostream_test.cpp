#include "../test_utils.hpp"
#include "../../includes/iostream/thread_safe_iostream.hpp"
#include <iostream>
#include <sstream>
#include <string>

void thread_safe_iostream_tests() {
	TEST_SECTION("ThreadSafeIOStream");

	// Redirect std::cout into a buffer so we can inspect what was written.
	std::ostringstream cap;
	std::streambuf* old = std::cout.rdbuf(cap.rdbuf());

	// a completed line is flushed with the prefix prepended
	threadSafeCout.setPrefix("[T] ");
	threadSafeCout << "hello\n";
	EXPECT(cap.str() == "[T] hello\n");
	cap.str("");

	// an incomplete line stays buffered until a newline arrives
	threadSafeCout.setPrefix("[P] ");
	threadSafeCout << "partial";
	EXPECT(cap.str() == "");                       // nothing flushed yet
	threadSafeCout << " end\n";
	EXPECT(cap.str() == "[P] partial end\n");
	cap.str("");

	// every completed line gets its own prefix
	threadSafeCout.setPrefix("# ");
	threadSafeCout << "a\nb\n";
	EXPECT(cap.str() == "# a\n# b\n");
	cap.str("");

	// std::endl flushes the buffered text with the prefix
	threadSafeCout.setPrefix("> ");
	threadSafeCout << "x" << std::endl;
	EXPECT(cap.str() == "> x\n");
	cap.str("");

	// values of different types are serialized in order
	threadSafeCout.setPrefix("");
	threadSafeCout << "n=" << 42 << "\n";
	EXPECT(cap.str() == "n=42\n");
	cap.str("");

	// Restore the real std::cout.
	std::cout.rdbuf(old);
}
