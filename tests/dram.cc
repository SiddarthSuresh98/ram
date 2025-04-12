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
		this->mem = new int;
		this->fetch = new int;
		this->expected = {0, 0, 0, 0};
		this->actual = this->d->view(0, 1)[0];
	}

	~DramFixture()
	{
		delete this->d;
		delete this->mem;
		delete this->fetch;
	}

	void
	wait_for_storage(int delay, std::function<int()> f)
	{
		for (int i = 0; i < delay; ++i) {
			int r = f();

			// check response
			CHECK(!r);
			// check for early modifications
			actual = d->view(0, 1)[0];
			REQUIRE(this->expected == this->actual);
		}
	}

	int delay;
	Dram *d;
	int *mem;
	int *fetch;
	std::array<signed int, LINE_SIZE> expected;
	std::array<signed int, LINE_SIZE> actual;
};

TEST_CASE_METHOD(DramFixture, "store 0th element in DELAY cycles", "[dram]")
{
	int r;
	signed int w;
	CHECK(expected == actual);

	w = 0x11223344;
	this->wait_for_storage(this->delay, [this, w]() { return this->d->write_word(this->mem, w, 0x0); });

	r = this->d->write_word(this->mem, w, 0x0);

	CHECK(r);
	expected.at(0) = w;
	actual = this->d->view(0, 1)[0];
	REQUIRE(expected == actual);
}

TEST_CASE_METHOD(DramFixture, "store 0th, 1st element in DELAY cycles, no conflict", "[dram]")
{
	int r;
	signed int w;
	CHECK(expected == actual);

	w = 0x11223344;
	this->wait_for_storage(this->delay, [this, w]() { return this->d->write_word(this->mem, w, 0x0); });

	r = d->write_word(this->mem, w, 0x0);
	REQUIRE(r);

	expected.at(0) = w;
	actual = d->view(0, 1)[0];
	REQUIRE(expected == actual);

	this->wait_for_storage(this->delay, [this, w]() { return this->d->write_word(this->fetch, w, 0x1); });

	r = d->write_word(this->fetch, w, 0x1);
	CHECK(r);

	actual = d->view(0, 1)[0];
	expected.at(1) = w;
	REQUIRE(expected == actual);
}

TEST_CASE_METHOD(DramFixture, "store 0th element in DELAY cycles with conflict", "[dram]")
{
	int r, i;
	signed int w;
	CHECK(expected == actual);

	w = 0x11223344;
	for (i = 0; i < this->delay; ++i) {
		r = this->d->write_word(this->mem, w, 0x0);
		CHECK(!r);
		r = this->d->write_word(this->fetch, w, 0x1);
		CHECK(!r);

		// check for early modifications
		actual = d->view(0, 1)[0];
		REQUIRE(expected == actual);
	}

	r = d->write_word(this->mem, w, 0x0);
	REQUIRE(r);

	expected.at(0) = w;
	actual = d->view(0, 1)[0];
	REQUIRE(expected == actual);

	this->wait_for_storage(this->delay, [this, w]() { return this->d->write_word(this->fetch, w, 0x1); });

	r = d->write_word(this->fetch, w, 0x1);
	CHECK(r);

	actual = d->view(0, 1)[0];
	expected.at(1) = w;
	REQUIRE(expected == actual);
}

TEST_CASE_METHOD(DramFixture, "store line in DELAY cycles", "[dram]")
{
	int r;
	signed int w;
	std::array<signed int, LINE_SIZE> buffer;
	CHECK(expected == actual);

	w = 0x11223344;
	buffer = {w, w + 1, w + 2, w + 3};
	this->wait_for_storage(
		this->delay, [this, w, buffer]() { return this->d->write_line(this->mem, buffer, 0x0); });

	r = d->write_line(this->mem, buffer, 0x0);
	CHECK(r);

	actual = d->view(0, 1)[0];
	expected = buffer;
	REQUIRE(expected == actual);
}

TEST_CASE_METHOD(DramFixture, "store line in DELAY cycles no conflict", "[dram]")
{
	int r;
	signed int w;
	std::array<signed int, LINE_SIZE> buffer;
	CHECK(expected == actual);

	w = 0x11223344;
	buffer = {w, w + 1, w + 2, w + 3};
	this->wait_for_storage(
		this->delay, [this, w, buffer]() { return this->d->write_line(this->mem, buffer, 0x0); });

	r = this->d->write_line(this->mem, buffer, 0x0);
	REQUIRE(r);

	expected = buffer;
	actual = d->view(0, 1)[0];
	REQUIRE(expected == actual);

	buffer = {w + 4, w + 5, w + 6, w + 7};
	this->wait_for_storage(
		this->delay, [this, w, buffer]() { return this->d->write_line(this->fetch, buffer, 0x1); });

	r = this->d->write_line(this->fetch, buffer, 0x1);
	CHECK(r);

	expected = buffer;
	actual = d->view(0, 1)[0];
	REQUIRE(expected == actual);
}

TEST_CASE_METHOD(DramFixture, "store line in DELAY cycles with conflict", "[dram]")
{
	int r, i;
	signed int w;
	std::array<signed int, LINE_SIZE> buffer;
	CHECK(expected == actual);

	w = 0x11223344;
	buffer = {w, w + 1, w + 2, w + 3};
	for (i = 0; i < this->delay; ++i) {
		r = this->d->write_line(this->mem, buffer, 0x0);
		CHECK(!r);
		r = d->write_line(this->fetch, buffer, 0x1);
		CHECK(!r);

		// check for early modifications
		actual = d->view(0, 1)[0];
		REQUIRE(expected == actual);
	}

	r = d->write_line(this->mem, buffer, 0x0);
	CHECK(r);

	actual = d->view(0, 1)[0];
	expected = buffer;
	REQUIRE(expected == actual);

	buffer = {w + 4, w + 5, w + 6, w + 7};
	this->wait_for_storage(
		this->delay, [this, w, buffer]() { return this->d->write_line(this->fetch, buffer, 0x1); });

	r = this->d->write_line(this->fetch, buffer, 0x1);
	CHECK(r);

	expected = buffer;
	actual = d->view(0, 1)[0];
	REQUIRE(expected == actual);
}

TEST_CASE_METHOD(
	DramFixture, "store line in DELAY cycles, read in DELAY cycles, no conflict", "[dram]")
{
	int r, i, addr;
	signed int w;
	CHECK(expected == actual);

	w = 0x11223311;
	addr = 0x0;
	expected = {w, w + 1, w + 2, w + 3};
	for (i = 0; i < this->delay; ++i) {
		r = d->write_line(this->mem, expected, addr);
		CHECK(!r);
	}
	r = d->write_line(this->mem, expected, addr);
	CHECK(r);

	for (i = 0; i < this->delay; ++i) {
		r = d->read_line(this->mem, addr, actual);

		CHECK(!r);
		REQUIRE(expected != actual);
	}

	r = d->read_line(this->mem, addr, actual);

	CHECK(r);
	REQUIRE(expected == actual);
}

TEST_CASE_METHOD(
	DramFixture, "store line in DELAY cycles, read in DELAY cycles with conflict", "[dram]")
{
	int r, i, addr;
	signed int w;
	CHECK(expected == actual);

	w = 0x11223311;
	addr = 0x0;
	expected = {w, w + 1, w + 2, w + 3};
	for (i = 0; i < delay; ++i) {
		r = d->write_line(this->mem, expected, addr);
		CHECK(!r);

		r = d->read_line(this->fetch, addr, actual);
		CHECK(!r);
	}
	r = d->write_line(this->mem, expected, addr);
	CHECK(r);

	for (i = 0; i < this->delay; ++i) {
		r = d->read_line(this->mem, addr, actual);

		CHECK(!r);
		REQUIRE(expected != actual);
	}

	r = d->read_line(this->mem, addr, actual);

	CHECK(r);
	REQUIRE(expected == actual);
}

TEST_CASE_METHOD(
	DramFixture,
	"store line in DELAY cycles, read one element at a time in DELAY cycles "
	"with conflict",
	"[dram]")
{
	int r;
	signed int w, a;
	int i, j, addr;
	CHECK(expected == actual);

	w = 0x11223311;
	a = 0x0;
	addr = 0x0;
	expected = {w, w + 1, w + 2, w + 3};
	for (i = 0; i < this->delay; ++i) {
		r = d->write_line(this->mem, expected, addr);
		CHECK(!r);
	}
	r = d->write_line(this->mem, expected, addr);
	CHECK(r);

	actual = d->view(0, 1)[0];
	REQUIRE(expected == actual);

	for (i = 0; i < LINE_SIZE; ++i) {
		for (j = 0; j < this->delay; ++j) {
			r = d->read_word(this->mem, addr, a);

			CHECK(!r);
			REQUIRE(0x0 == a);
		}
		r = d->read_word(this->mem, addr++, a);
		CHECK(r);
		REQUIRE(w++ == a);

		a = 0;
	}
}
