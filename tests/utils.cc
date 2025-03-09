#include "utils.h"
#include "definitions.h"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Parse arbitrary fields # one", "[cache]")
{
	int tag, index, offset;
	int address = 0b111110001010101;
	get_bit_fields(address, &tag, &index, &offset);
	CHECK(tag == 0b11111000);
	CHECK(index == 0b10101);
	CHECK(offset == 0b01);
}

TEST_CASE("Parse arbitrary fields # two", "[cache]")
{
	int tag, index, offset;
	int address = 0b000110100111011;
	get_bit_fields(address, &tag, &index, &offset);
	CHECK(tag == 0b00011010);
	CHECK(index == 0b01110);
	CHECK(offset == 0b11);
}
