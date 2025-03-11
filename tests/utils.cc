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

TEST_CASE("wrap address outside upper bound", "[utils]")
{
	int address = MEM_LINES + 25;
	int wrapped = wrap_address(address);
	REQUIRE(wrapped == 25);
}

TEST_CASE("wrap address inside upper bound", "[utils]")
{
	int address = MEM_LINES - 25;
	int wrapped = wrap_address(address);
	REQUIRE(wrapped == MEM_LINES - 25);
}

TEST_CASE("wrap address at upper bound", "[utils]")
{
	int address = MEM_LINES;
	int wrapped = wrap_address(address);
	REQUIRE(wrapped == 0);
}

TEST_CASE("wrap address lower than 0 with magnitude lesser than mem size", "[utils]")
{
	int address = -10;
	int wrapped = wrap_address(address);
	REQUIRE(wrapped == MEM_LINES - 10);
}

TEST_CASE("wrap address lower than 0 but with magnitude greater than mem size", "[utils]")
{
	int address = -(MEM_LINES + 10);
	int wrapped = wrap_address(address);
	REQUIRE(wrapped == MEM_LINES - 10);
}

TEST_CASE("wrap address at 0", "[utils]")
{
	int address = 0;
	int wrapped = wrap_address(address);
	REQUIRE(wrapped == 0);
}
