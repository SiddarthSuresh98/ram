#include "c11.h"
#include "cache.h"
#include "dram.h"
#include "storage.h"
#include <catch2/catch_test_macros.hpp>
#include <iostream>

/**
 * One way associative, two level
 * Assuming that each address is 14 bits (16384 word address space):
 * LEVEL1: OFFSET=2, INDEX=5(32), TAG=7
 * LEVEL2: OFFSET=2, INDEX=7(128), TAG=5
 */
class C21 : public C11
{
  public:
	C21() : C11()
	{
		this->c2 = new Cache(new Dram(this->m_delay), 7, 0, this->c_delay);
		this->c = new Cache(this->c2, 5, 0, this->c_delay);
	}

	Cache *c2;
};

// TEST_CASE_METHOD(C21, "store 32th, 33rd element in DELAY cycles",

TEST_CASE_METHOD(C21, "store 32th, 96th element in DELAY cycles, evict to level 2", "[2level_cache]")
{
	int r;
	signed int w;
	CHECK(expected == actual);

	w = 0x11223344;
	// delay + 1 due to internal logic, when mem
	// finishes handle_miss still returns 'blocked'
	this->wait_then_do(this->m_delay + (this->c_delay * 2) + 2, [this, w]() {
		return this->c->write_word(this->mem, w, 0b10000000);
	});

	r = this->c->write_word(this->mem, w, 0b10000000);
	CHECK(r);

	// check level 2
	// note this is write-back == no write
	actual = this->c2->view(32, 1)[0];
	REQUIRE(expected == actual);

	// check level 1
	expected.at(0) = w;
	actual = this->c->view(0, 1)[0];
	REQUIRE(expected == actual);

	// wait = evict
	this->wait_then_do(this->c_delay + 1, [this, w]() {
		return this->c->write_word(this->mem, w, 0b110000000);
	});

	// check level 2
	actual = this->c2->view(32, 1)[0];
	REQUIRE(expected == actual);

	// read in line
	this->wait_then_do(this->m_delay + this->c_delay + 1, [this, w]() {
		return this->c->write_word(this->mem, w, 0b110000000);
	});

	expected.at(0) = 0;
	// perform write
	this->wait_then_do(this->c_delay + 1, [this, w]() {
		return this->c->write_word(this->mem, w, 0b110000000);
	});

	r = this->c->write_word(this->mem, w, 0b110000000);
	CHECK(r);

	// check level 2
	actual = this->c2->view(96, 1)[0];
	REQUIRE(expected == actual);
	expected.at(0) = w;
	actual = this->c2->view(32, 1)[0];
	REQUIRE(expected == actual);

	// check level 1
	actual = this->c->view(0, 1)[0];
	REQUIRE(expected == actual);
}
