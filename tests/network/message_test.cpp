#include "../test_utils.hpp"
#include "../../includes/network/message.hpp"
#include <string>
#include <vector>

void message_tests() {
	TEST_SECTION("Message");

	// type() returns the constructor type
	{
		Message m(7);
		EXPECT(m.type() == 7);
	}

	// round-trip a single scalar
	{
		Message m(1);
		m << int(12345);
		int out = 0;
		m >> out;
		EXPECT(out == 12345);
	}

	// round-trip multiple mixed scalars, read in write order (chaining)
	{
		Message m(1);
		m << int(7) << double(2.5) << char('z');
		int i = 0;
		double d = 0;
		char c = 0;
		m >> i >> d >> c;
		EXPECT(i == 7);
		EXPECT(d == 2.5);
		EXPECT(c == 'z');
	}

	// round-trip a std::string (length-prefixed overload)
	{
		Message m(2);
		m << std::string("hello world");
		std::string out;
		m >> out;
		EXPECT(out == "hello world");
	}

	// mix a string with scalars
	{
		Message m(3);
		m << int(42) << std::string("ada") << double(1.5);
		int n = 0;
		std::string s;
		double d = 0;
		m >> n >> s >> d;
		EXPECT(n == 42);
		EXPECT(s == "ada");
		EXPECT(d == 1.5);
	}

	// clear() empties the payload but keeps the type
	{
		Message m(9);
		m << int(1) << int(2);
		m.clear();
		EXPECT(m.type() == 9);
		int x = 0;
		EXPECT_THROW(m >> x);   // nothing left to read
	}

	// getMessageBuffer + uploadBuffer transfer the payload between messages
	{
		Message src(5);
		src << int(9) << std::string("hi");
		DataBuffer buf = src.getMessageBuffer();
		std::vector<std::byte> raw(buf.data(), buf.data() + buf.size());

		Message dst(5);
		dst.uploadBuffer(raw);
		int n = 0;
		std::string s;
		dst >> n >> s;
		EXPECT(n == 9);
		EXPECT(s == "hi");
	}
}
