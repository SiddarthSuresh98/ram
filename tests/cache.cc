#include "cache.h"
#include "dram.h"
#include <catch2/catch_test_macros.hpp>

class CacheFixture
{
  public:
	CacheFixture()
	{
		this->m_delay = 4;
		this->c_delay = 2;
		this->d = new Dram(this->m_delay);
		this->c = new Cache(this->d, this->c_delay);
		this->mem = new int;
		this->fetch = new int;
		this->expected = {0, 0, 0, 0};
		this->actual = this->c->view(0, 1)[0];
	}

	~CacheFixture()
	{
		delete this->c;
		delete this->mem;
		delete this->fetch;
	}

	/**
	 * An operation that is done a lot.
	 */
	void
	wait_for_storage(int delay, int expected, std::function<int()> f)
	{
		for (int i = 0; i < delay; ++i) {
			int r = f();

			// check response
			CHECK(r == expected);
			// check for early modifications
			actual = c->view(0, 1)[0];
			REQUIRE(this->expected == this->actual);
		}
	}

	int m_delay;
	int c_delay;
	Cache *c;
	Dram *d;
	int *mem;
	int *fetch;
	std::array<signed int, LINE_SIZE> expected;
	std::array<signed int, LINE_SIZE> actual;
};

TEST_CASE_METHOD(CacheFixture, "store 0th element in DELAY cycles", "[dram]")
{
	int r;
	signed int w;
	CHECK(expected == actual);

	w = 0x11223344;
	// delay + 1 due to internal logic, when mem
	// finishes handle_miss still returns 'blocked'
	this->wait_for_storage(this->m_delay + this->c_delay + 1, 0, [this, w]() {
		return this->c->write_word(this->mem, w, 0b0);
	});

	r = c->write_word(this->mem, w, 0b0);
	CHECK(r);

	actual = this->d->view(0, 1)[0];
	// we do NOT write back now!
	REQUIRE(expected == actual);

	expected.at(0) = w;
	actual = c->view(0, 1)[0];
	REQUIRE(expected == actual);
}

TEST_CASE_METHOD(CacheFixture, "store 0th, 1st element in DELAY cycles, with conflict", "[cache]")
{
	signed int w;
	int r, i;
	CHECK(expected == actual);

	w = 0x11223344;
	// delay + 1 due to internal logic, when mem
	// finishes handle_miss still returns 'blocked'
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

	actual = d->view(0, 1)[0];
	// we do NOT write back now!
	REQUIRE(expected == actual);

	expected.at(0) = w;
	actual = c->view(0, 1)[0];
	REQUIRE(expected == actual);

	// this should have been loaded already!
	this->wait_for_storage(
		this->c_delay, 0, [this, w]() { return this->c->write_word(this->fetch, w, 0b1); });

	r = c->write_word(this->fetch, w, 0b1);
	CHECK(r);

	expected.at(1) = w;
	actual = c->view(0, 1)[0];
	REQUIRE(expected == actual);
}

TEST_CASE_METHOD(
	CacheFixture, "store 0th, 1st element different tags, in DELAY cycles, no conflict", "[cache]")
{
	int r;
	signed int w;
	CHECK(expected == actual);

	w = 0x11223344;
	// delay + 1 due to internal logic, when mem
	// finishes handle_miss still returns 'blocked'
	this->wait_for_storage(this->m_delay + this->c_delay + 1, 0, [this, w]() {
		return this->c->write_word(this->mem, w, 0b0);
	});

	r = c->write_word(this->mem, w, 0b0);
	CHECK(r);

	expected.at(0) = w;
	actual = c->view(0, 1)[0];
	REQUIRE(expected == actual);

	// write back to memory
	// fetch new address (don't run the completion cycle yet)
	this->wait_for_storage(this->m_delay + this->m_delay + 1, 0, [this, w]() {
		return this->c->write_word(this->fetch, w, 0b10000001);
	});

	// after the fetch, this cache line should be empty
	this->c->write_word(this->fetch, w, 0b10000001);
	CHECK(r);

	expected.at(0) = 0;
	actual = c->view(0, 1)[0];
	CHECK(expected == actual);

	this->wait_for_storage(
		this->c_delay, 0, [this, w]() { return this->c->write_word(this->fetch, w, 0b10000001); });

	r = c->write_word(this->fetch, w, 0b10000001);
	CHECK(r);

	expected.at(0) = 0;
	expected.at(1) = w;
	actual = c->view(0, 1)[0];
	REQUIRE(expected == actual);
}
