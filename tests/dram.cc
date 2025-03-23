#include "dram.h"
#include <array>
#include <catch2/catch_test_macros.hpp>

class DramFixture
{
  public:
	DramFixture()
	{
		this->delay = 3;
		this->d = new Dram(this->delay);
		this->expected = {0, 0, 0, 0};
		this->actual = this->d->view(0, 1)[0];
	}

	~DramFixture() { delete this->d; }

	/**
	 * An operation that is done a lot.
	 */
	void
	wait_for_storage(int delay, Response expected, std::function<Response()> f)
	{
		for (int i = 0; i < delay; ++i) {
			Response r = f();

			// check response
			CHECK(r == expected);
			// check for early modifications
			actual = d->view(0, 1)[0];
			REQUIRE(this->expected == this->actual);
		}
	}

	int delay;
	Dram *d;
	std::array<signed int, LINE_SIZE> expected;
	std::array<signed int, LINE_SIZE> actual;
};

TEST_CASE_METHOD(DramFixture, "store 0th element in DELAY cycles", "[dram]")
{
	Response r;
	signed int w;
	CHECK(expected == actual);

	w = 0x11223344;
	this->wait_for_storage(this->delay, WAIT, [this, w]() {
		return this->d->write_word(MEM, w, 0x0);
	});

	r = this->d->write_word(MEM, w, 0x0);

	CHECK(r == OK);
	expected.at(0) = w;
	actual = this->d->view(0, 1)[0];
	REQUIRE(expected == actual);
}

TEST_CASE_METHOD(
	DramFixture,
	"store 0th, 1st element in DELAY cycles, no conflict",
	"[dram]")
{
	Response r;
	signed int w;
	CHECK(expected == actual);

	w = 0x11223344;
	this->wait_for_storage(this->delay, WAIT, [this, w]() {
		return this->d->write_word(MEM, w, 0x0);
	});

	r = d->write_word(MEM, w, 0x0);
	REQUIRE(r == OK);

	expected.at(0) = w;
	actual = d->view(0, 1)[0];
	REQUIRE(expected == actual);

	this->wait_for_storage(this->delay, WAIT, [this, w]() {
		return this->d->write_word(FETCH, w, 0x1);
	});

	r = d->write_word(FETCH, w, 0x1);
	CHECK(r == OK);

	actual = d->view(0, 1)[0];
	expected.at(1) = w;
	REQUIRE(expected == actual);
}

TEST_CASE_METHOD(
	DramFixture, "store 0th element in DELAY cycles with conflict", "[dram]")
{
	Response r;
	signed int w;
	int i;
	CHECK(expected == actual);

	w = 0x11223344;
	for (i = 0; i < this->delay; ++i) {
		r = this->d->write_word(MEM, w, 0x0);
		CHECK(r == WAIT);
		r = this->d->write_word(FETCH, w, 0x1);
		CHECK(r == WAIT);

		// check for early modifications
		actual = d->view(0, 1)[0];
		REQUIRE(expected == actual);
	}

	r = d->write_word(MEM, w, 0x0);
	REQUIRE(r == OK);

	expected.at(0) = w;
	actual = d->view(0, 1)[0];
	REQUIRE(expected == actual);

	this->wait_for_storage(this->delay, WAIT, [this, w]() {
		return this->d->write_word(FETCH, w, 0x1);
	});

	r = d->write_word(FETCH, w, 0x1);
	CHECK(r == OK);

	actual = d->view(0, 1)[0];
	expected.at(1) = w;
	REQUIRE(expected == actual);
}

TEST_CASE_METHOD(DramFixture, "store line in DELAY cycles", "[dram]")
{
	Response r;
	signed int w;
	std::array<signed int, LINE_SIZE> buffer;
	CHECK(expected == actual);

	w = 0x11223344;
	buffer = {w, w + 1, w + 2, w + 3};
	this->wait_for_storage(this->delay, WAIT, [this, w, buffer]() {
		return this->d->write_line(MEM, buffer, 0x0);
	});

	r = d->write_line(MEM, buffer, 0x0);
	CHECK(r == OK);

	actual = d->view(0, 1)[0];
	expected = buffer;
	REQUIRE(expected == actual);
}

TEST_CASE_METHOD(
	DramFixture, "store line in DELAY cycles no conflict", "[dram]")
{
	Response r;
	signed int w;
	std::array<signed int, LINE_SIZE> buffer;
	CHECK(expected == actual);

	w = 0x11223344;
	buffer = {w, w + 1, w + 2, w + 3};
	this->wait_for_storage(this->delay, WAIT, [this, w, buffer]() {
		return this->d->write_line(MEM, buffer, 0x0);
	});

	r = this->d->write_line(MEM, buffer, 0x0);
	REQUIRE(r == OK);

	expected = buffer;
	actual = d->view(0, 1)[0];
	REQUIRE(expected == actual);

	buffer = {w + 4, w + 5, w + 6, w + 7};
	this->wait_for_storage(this->delay, WAIT, [this, w, buffer]() {
		return this->d->write_line(FETCH, buffer, 0x1);
	});

	r = this->d->write_line(FETCH, buffer, 0x1);
	CHECK(r == OK);

	expected = buffer;
	actual = d->view(0, 1)[0];
	REQUIRE(expected == actual);
}

TEST_CASE_METHOD(
	DramFixture, "store line in DELAY cycles with conflict", "[dram]")
{
	Response r;
	signed int w;
	int i;
	std::array<signed int, LINE_SIZE> buffer;
	CHECK(expected == actual);

	w = 0x11223344;
	buffer = {w, w + 1, w + 2, w + 3};
	for (i = 0; i < this->delay; ++i) {
		r = this->d->write_line(MEM, buffer, 0x0);
		CHECK(r == WAIT);
		r = d->write_line(FETCH, buffer, 0x1);
		CHECK(r == WAIT);

		// check for early modifications
		actual = d->view(0, 1)[0];
		REQUIRE(expected == actual);
	}

	r = d->write_line(MEM, buffer, 0x0);
	CHECK(r == OK);

	actual = d->view(0, 1)[0];
	expected = buffer;
	REQUIRE(expected == actual);

	buffer = {w + 4, w + 5, w + 6, w + 7};
	this->wait_for_storage(this->delay, WAIT, [this, w, buffer]() {
		return this->d->write_line(FETCH, buffer, 0x1);
	});

	r = this->d->write_line(FETCH, buffer, 0x1);
	CHECK(r == OK);

	expected = buffer;
	actual = d->view(0, 1)[0];
	REQUIRE(expected == actual);
}

TEST_CASE_METHOD(
	DramFixture,
	"store line in DELAY cycles, read in DELAY cycles, no conflict",
	"[dram]")
{
	Response r;
	signed int w;
	int i, addr;
	CHECK(expected == actual);

	w = 0x11223311;
	addr = 0x0;
	expected = {w, w + 1, w + 2, w + 3};
	for (i = 0; i < this->delay; ++i) {
		r = d->write_line(MEM, expected, addr);
		CHECK(r == WAIT);
	}
	r = d->write_line(MEM, expected, addr);
	CHECK(r == OK);

	for (i = 0; i < this->delay; ++i) {
		r = d->read_line(MEM, addr, actual);

		CHECK(r == WAIT);
		REQUIRE(expected != actual);
	}

	r = d->read_line(MEM, addr, actual);

	CHECK(r == OK);
	REQUIRE(expected == actual);
}

TEST_CASE_METHOD(
	DramFixture,
	"store line in DELAY cycles, read in DELAY cycles with conflict",
	"[dram]")
{
	Response r;
	signed int w;
	int i, addr;
	CHECK(expected == actual);

	w = 0x11223311;
	addr = 0x0;
	expected = {w, w + 1, w + 2, w + 3};
	for (i = 0; i < delay; ++i) {
		r = d->write_line(MEM, expected, addr);
		CHECK(r == WAIT);

		r = d->read_line(FETCH, addr, actual);
		CHECK(r == WAIT);
	}
	r = d->write_line(MEM, expected, addr);
	CHECK(r == OK);

	for (i = 0; i < this->delay; ++i) {
		r = d->read_line(MEM, addr, actual);

		CHECK(r == WAIT);
		REQUIRE(expected != actual);
	}

	r = d->read_line(MEM, addr, actual);

	CHECK(r == OK);
	REQUIRE(expected == actual);
}

TEST_CASE_METHOD(
	DramFixture,
	"store line in DELAY cycles, read one element at a time in DELAY cycles "
	"with conflict",
	"[dram]")
{
	Response r;
	signed int w, a;
	int i, j, addr;
	CHECK(expected == actual);

	w = 0x11223311;
	a = 0x0;
	addr = 0x0;
	expected = {w, w + 1, w + 2, w + 3};
	for (i = 0; i < this->delay; ++i) {
		r = d->write_line(MEM, expected, addr);
		CHECK(r == WAIT);
	}
	r = d->write_line(MEM, expected, addr);
	CHECK(r == OK);

	actual = d->view(0, 1)[0];
	REQUIRE(expected == actual);

	for (i = 0; i < LINE_SIZE; ++i) {
		for (j = 0; j < this->delay; ++j) {
			r = d->read_word(MEM, addr, a);

			CHECK(r == WAIT);
			REQUIRE(0x0 == a);
		}
		r = d->read_word(MEM, addr++, a);
		CHECK(r == OK);
		REQUIRE(w++ == a);

		a = 0;
	}
}

TEST_CASE_METHOD(DramFixture, "Sidedoor bypasses delay", "[dram]")
{
	Response r;
	signed int w;
	CHECK(expected == actual);

	w = 0x11223344;
	r = this->d->write_word(SIDE, w, 0x0);
	CHECK(r == OK);

	expected.at(0) = w;
	actual = d->view(0, 1)[0];
	REQUIRE(expected == actual);
}
