#include "utils.h"
#include "definitions.h"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Parse arbitrary fields # one", "[utils]")
{
	int tag, index, offset;
	int address = 0b0001010101;
	get_cache_fields(address, &tag, &index, &offset);
	CHECK(tag == 0b000);
	CHECK(index == 0b10101);
	CHECK(offset == 0b01);
}

TEST_CASE("Parse arbitrary fields # two", "[utils]")
{
	int tag, index, offset;
	int address = 0b0100111011;
	get_cache_fields(address, &tag, &index, &offset);
	CHECK(tag == 0b010);
	CHECK(index == 0b01110);
	CHECK(offset == 0b11);
}

TEST_CASE("Parse invalid type", "[utils]")
{
	signed int s1 = 0, s2 = 0, s3 = 0;
	unsigned int type = 0, opcode = 0;

	s1 = 0x00FF00FF;
	get_instr_fields(s1, s2, s3, type, opcode);

	REQUIRE(type == 0b11);
	// behavior does nothing
	CHECK(s1 == 0x00FF00FF);
	CHECK(s2 == 0b0);
	CHECK(s3 == 0b0);
	CHECK(opcode == 0b0);
}

TEST_CASE("Parse arbitrary r-type # one", "[utils]")
{
	signed int s1 = 0, s2 = 0, s3 = 0;
	unsigned int type = 0, opcode = 0;

	s1 = 0xCCCCCCCC;
	get_instr_fields(s1, s2, s3, type, opcode);

	REQUIRE(type == 0b00);
	CHECK(s1 == 0b11001);
	CHECK(s2 == 0b01100);
	CHECK(s3 == 0b00110);
	CHECK(opcode == 0b10011);
}

TEST_CASE("Parse arbitrary r-type # two", "[utils]")
{
	signed int s1 = 0, s2 = 0, s3 = 0;
	unsigned int type = 0, opcode = 0;

	s1 = 0x99AABBCC;
	get_instr_fields(s1, s2, s3, type, opcode);

	REQUIRE(type == 0b00);
	CHECK(s1 == 0b10111);
	CHECK(s2 == 0b01011);
	CHECK(s3 == 0b10101);
	CHECK(opcode == 0b10011);
}

TEST_CASE("Parse arbitrary i-type # one", "[utils]")
{
	signed int s1 = 0, s2 = 0, s3 = 0;
	unsigned int type = 0, opcode = 0;

	s1 = 0xDDDDDDDD;
	get_instr_fields(s1, s2, s3, type, opcode);

	REQUIRE(type == 0b01);
	CHECK(s1 == 0b10111);
	CHECK(s2 == 0b11011);
	CHECK(s3 == 0xDDDD);
	CHECK(opcode == 0b0111);
}

TEST_CASE("Parse arbitrary i-type # two", "[utils]")
{
	signed int s1 = 0, s2 = 0, s3 = 0;
	unsigned int type = 0, opcode = 0;

	s1 = 0xAABBCCDD;
	get_instr_fields(s1, s2, s3, type, opcode);

	REQUIRE(type == 0b01);
	CHECK(s1 == 0b10011);
	CHECK(s2 == 0b11001);
	CHECK(s3 == 0xAABB);
	CHECK(opcode == 0b0111);
}

TEST_CASE("Parse arbitrary j-type # one", "[utils]")
{
	signed int s1 = 0, s2 = 0, s3 = 0;
	unsigned int type = 0, opcode = 0;

	s1 = 0xEEEEEEEE;
	get_instr_fields(s1, s2, s3, type, opcode);

	REQUIRE(type == 0b10);
	CHECK(s1 == 0b11011);
	CHECK(s2 == 0b111011101110111011101);	
	CHECK(opcode == 0b1011);
	// behavior does nothing
	CHECK(s3 == 0b0);
}

TEST_CASE("Parse arbitrary j-type # two", "[utils]")
{
	signed int s1 = 0, s2 = 0, s3 = 0;
	unsigned int type = 0, opcode = 0;

	s1 = 0xBBCCDDEE;
	get_instr_fields(s1, s2, s3, type, opcode);

	REQUIRE(type == 0b10);
	CHECK(s1 == 0b10111);
	CHECK(s2 == 0b101110111100110011011);	
	CHECK(opcode == 0b1011);
	// behavior does nothing
	CHECK(s3 == 0b0);
}

TEST_CASE("wrap address outside upper bound", "[utils]")
{
	int address = MEM_WORDS + 25;
	int wrapped = wrap_address(address);
	REQUIRE(wrapped == 25);
}

TEST_CASE("wrap address inside upper bound", "[utils]")
{
	int address = MEM_WORDS - 25;
	int wrapped = wrap_address(address);
	REQUIRE(wrapped == MEM_WORDS - 25);
}

TEST_CASE("wrap address at upper bound", "[utils]")
{
	int address = MEM_WORDS;
	int wrapped = wrap_address(address);
	REQUIRE(wrapped == 0);
}

TEST_CASE("wrap address lower than 0 with magnitude lesser than mem size", "[utils]")
{
	int address = -10;
	int wrapped = wrap_address(address);
	REQUIRE(wrapped == MEM_WORDS - 10);
}

TEST_CASE("wrap address lower than 0 but with magnitude greater than mem size", "[utils]")
{
	int address = -(MEM_WORDS + 10);
	int wrapped = wrap_address(address);
	REQUIRE(wrapped == MEM_WORDS - 10);
}

TEST_CASE("wrap address at 0", "[utils]")
{
	int address = 0;
	int wrapped = wrap_address(address);
	REQUIRE(wrapped == 0);
}
