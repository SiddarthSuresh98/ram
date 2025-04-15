#include "c11.h"
#include "cache.h"
#include "dram.h"
#include <catch2/catch_test_macros.hpp>

TEST_CASE_METHOD(C11, "store 0th element in DELAY cycles", "[dram]")
{
	int r;
	signed int w;
	CHECK(expected == actual);

	w = 0x11223344;
	// delay + 1 due to internal logic, when mem
	// finishes is_address_missing still returns '1'
	this->wait_then_do(this->m_delay + this->c_delay + 1, [this, w]() {
		return this->c->write_word(this->mem, w, 0b0);
	});

	r = c->write_word(this->mem, w, 0b0);
	CHECK(r);

	expected.at(0) = w;
	actual = c->view(0, 1)[0];
	REQUIRE(expected == actual);
}

TEST_CASE_METHOD(C11, "store 0th, 1st element in DELAY cycles, with conflict", "[cache]")
{
	signed int w;
	int r, i;
	CHECK(expected == actual);

	w = 0x11223344;
	// delay + 1 due to internal logic, when mem
	// finishes is_address_missing still returns '1'
	for (i = 0; i < this->m_delay + this->c_delay + 1; ++i) {
		r = c->write_word(this->mem, w, 0b0);
		CHECK(!r);
		r = c->write_word(this->fetch, w, 0b1);
		CHECK(!r);

		// check for early modifications
		actual = c->view(0, 1)[0];
		REQUIRE(this->expected == this->actual);
	}

	r = c->write_word(this->mem, w, 0b0);
	CHECK(r);

	expected.at(0) = w;
	actual = c->view(0, 1)[0];
	REQUIRE(expected == actual);

	// this should have been loaded already!
	this->wait_then_do(
		this->c_delay, [this, w]() { return this->c->write_word(this->fetch, w, 0b1); });

	r = c->write_word(this->fetch, w, 0b1);
	CHECK(r);

	expected.at(1) = w;
	actual = c->view(0, 1)[0];
	REQUIRE(expected == actual);
}

TEST_CASE_METHOD(
	C11, "store 0th, 1st element different tags, in DELAY cycles, no conflict", "[cache]")
{
	int r;
	signed int w;
	CHECK(expected == actual);

	w = 0x11223344;
	// delay + 1 due to internal logic, when mem
	// finishes is_address_missing still returns '1'
	this->wait_then_do(this->m_delay + this->c_delay + 1, [this, w]() {
		return this->c->write_word(this->mem, w, 0b0);
	});

	r = c->write_word(this->mem, w, 0b0);
	CHECK(r);

	expected.at(0) = w;
	actual = c->view(0, 1)[0];
	REQUIRE(expected == actual);

	// write back to memory
	// fetch new address (don't run the completion cycle yet)
	this->wait_then_do(this->m_delay + this->m_delay + 1, [this, w]() {
		return this->c->write_word(this->fetch, w, 0b10000001);
	});

	// after the fetch, this cache line should be empty
	this->c->write_word(this->fetch, w, 0b10000001);
	CHECK(r);

	expected.at(0) = 0;
	actual = c->view(0, 1)[0];
	CHECK(expected == actual);

	this->wait_then_do(
		this->c_delay, [this, w]() { return this->c->write_word(this->fetch, w, 0b10000001); });

	r = c->write_word(this->fetch, w, 0b10000001);
	CHECK(r);

	expected.at(0) = 0;
	expected.at(1) = w;
	actual = c->view(0, 1)[0];
	REQUIRE(expected == actual);
}
