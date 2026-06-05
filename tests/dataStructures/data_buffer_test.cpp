#include "../test_utils.hpp"
#include "../../includes/dataStructures/data_buffer.hpp"

void data_buffer_tests() {
	TEST_SECTION("DataBuffer");

	// size grows by sizeof of each pushed value
	DataBuffer buf;
	EXPECT(buf.size() == 0);
	buf << int(42);
	EXPECT(buf.size() == sizeof(int));
	buf << double(3.14);
	EXPECT(buf.size() == sizeof(int) + sizeof(double));

	// round-trip a single int
	DataBuffer b1;
	b1 << int(12345);
	int out = 0;
	b1 >> out;
	EXPECT(out == 12345);

	// round-trip multiple mixed types, read in push order
	DataBuffer b2;
	b2 << int(7) << double(2.5) << char('z');
	int i = 0;
	double d = 0;
	char c = 0;
	b2 >> i >> d >> c;
	EXPECT(i == 7);
	EXPECT(d == 2.5);
	EXPECT(c == 'z');

	// reset() rewinds the read offset so the same data reads again
	DataBuffer b3;
	b3 << int(99);
	int first = 0, second = 0;
	b3 >> first;
	b3.reset();
	b3 >> second;
	EXPECT(first == 99);
	EXPECT(second == 99);

	// clear() empties the buffer
	DataBuffer b4;
	b4 << int(1) << int(2);
	b4.clear();
	EXPECT(b4.size() == 0);

	// reading past the end throws
	DataBuffer b5;
	b5 << int(5);
	int sink = 0;
	b5 >> sink;              // consumes the only value
	EXPECT_THROW(b5 >> sink); // nothing left -> throw

	// setBuffer replaces contents and resets the offset
	DataBuffer src;
	src << int(2024);
	std::vector<std::byte> raw(src.data(), src.data() + src.size());
	DataBuffer dst;
	dst << int(0);           // some prior content
	dst.setBuffer(raw);
	int year = 0;
	dst >> year;
	EXPECT(year == 2024);
}
