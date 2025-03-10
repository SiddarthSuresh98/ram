#include "cache.h"
#include "definitions.h"
#include "dram.h"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Constructor singleton cache", "[cache]")
{
	Cache *c = new Cache(nullptr, 0);
	std::array<signed int, LINE_SIZE> expected = {0, 0, 0, 0};
	std::array<signed int, LINE_SIZE> actual = c->view(0, 1)[0];
	REQUIRE(expected == actual);
	delete c;
}

TEST_CASE("no delay stores instantly", "[cache]")
{
	int delay = 0;
	Dram *d = new Dram(MEM_SIZE, delay);
	Cache *c = new Cache(d, delay);
	std::array<signed int, LINE_SIZE> expected = {0, 0, 0, 0};
	std::array<signed int, LINE_SIZE> actual = d->view(0, 1)[0];
	CHECK(expected == actual);

	signed int w = 0x11223344;

	Response r;

	r = c->write(MEM, w, 0x0000000000000);
	CHECK(r == OK);
	d->resolve();
	c->resolve();

	actual = d->view(0, 1)[0];
	// we do NOT write back now!
	REQUIRE(expected == actual);

	expected.at(0) = w;
	actual = c->view(0, 1)[0];
	REQUIRE(expected == actual);

	delete d;
	delete c;
}

TEST_CASE("cache takes \"forever\"", "[cache]")
{
	int delay = 0;
	Dram *d = new Dram(MEM_SIZE, delay);
	Cache *c = new Cache(d, delay + 2);
	std::array<signed int, LINE_SIZE> expected = {0, 0, 0, 0};
	std::array<signed int, LINE_SIZE> actual = d->view(0, 1)[0];
	CHECK(expected == actual);

	signed int w = 0x11223344;

	int i;
	Response r;
	for (i = 0; i < delay + 2; ++i) {
		r = c->write(MEM, w, 0x0000000000000);
		CHECK(r == WAIT); // WAIT

		actual = c->view(0, 1)[0];
		REQUIRE(expected == actual);
		c->resolve();
		d->resolve();
	}

	r = c->write(MEM, w, 0x0000000000000);
	CHECK(r == OK);
	d->resolve();

	actual = d->view(0, 1)[0];
	// we do NOT write back now!
	REQUIRE(expected == actual);

	expected.at(0) = w;
	actual = c->view(0, 1)[0];
	REQUIRE(expected == actual);

	delete d;
	delete c;
}

TEST_CASE("dram takes \"forever\"", "[cache]")
{
	int delay = 0;
	Dram *d = new Dram(MEM_SIZE, delay + 2);
	Cache *c = new Cache(d, delay);
	std::array<signed int, LINE_SIZE> expected = {0, 0, 0, 0};
	std::array<signed int, LINE_SIZE> actual = d->view(0, 1)[0];
	CHECK(expected == actual);

	signed int w = 0x11223344;

	int i;
	Response r;
	for (i = 0; i < delay + 2; ++i) {
		r = c->write(MEM, w, 0x0000000000000);
		CHECK(r == BLOCKED); // BLOCKED

		actual = c->view(0, 1)[0];
		REQUIRE(expected == actual);
		c->resolve();
		d->resolve();
	}

	r = c->write(MEM, w, 0x0000000000000);
	CHECK(r == OK);
	d->resolve();

	actual = d->view(0, 1)[0];
	// we do NOT write back now!
	REQUIRE(expected == actual);

	expected.at(0) = w;
	actual = c->view(0, 1)[0];
	REQUIRE(expected == actual);

	delete d;
	delete c;
}

TEST_CASE("dram and cache take \"forever\"", "[cache]")
{
	int delay = 2;
	Dram *d = new Dram(MEM_SIZE, delay + 2);
	Cache *c = new Cache(d, delay);
	std::array<signed int, LINE_SIZE> expected = {0, 0, 0, 0};
	std::array<signed int, LINE_SIZE> actual = d->view(0, 1)[0];
	CHECK(expected == actual);

	signed int w = 0x11223344;

	int i;
	Response r;
	for (i = 0; i < delay + 2; ++i) {
		r = c->write(MEM, w, 0x0000000000000);
		CHECK(r == BLOCKED); // BLOCKED

		actual = c->view(0, 1)[0];
		REQUIRE(expected == actual);
		c->resolve();
		d->resolve();
	}

	for (i = 0; i < delay; ++i) {
		r = c->write(MEM, w, 0x0000000000000);
		CHECK(r == WAIT); // WAIT

		actual = c->view(0, 1)[0];
		REQUIRE(expected == actual);
		c->resolve();
		d->resolve();
	}

	r = c->write(MEM, w, 0x0000000000000);
	CHECK(r == OK);
	c->resolve();
	d->resolve();

	actual = d->view(0, 1)[0];
	// we do NOT write back now!
	REQUIRE(expected == actual);

	expected.at(0) = w;
	actual = c->view(0, 1)[0];
	REQUIRE(expected == actual);

	delete d;
	delete c;
}

TEST_CASE("dram takes \"forever\", two concurrent requests same index", "[cache]")
{
	int delay = 0;
	Dram *d = new Dram(MEM_SIZE, delay + 2);
	Cache *c = new Cache(d, delay);
	std::array<signed int, LINE_SIZE> expected = {0, 0, 0, 0};
	std::array<signed int, LINE_SIZE> actual = d->view(0, 1)[0];
	CHECK(expected == actual);

	signed int w = 0x11223344;

	int i;
	Response r;
	for (i = 0; i < delay + 2; ++i) {
		r = c->write(MEM, w, 0x0000000000000);
		CHECK(r == BLOCKED); // BLOCKED

		r = c->write(FETCH, w, 0x0000000000001);
		CHECK(r == WAIT); // WAIT

		actual = c->view(0, 1)[0];
		REQUIRE(expected == actual);
		c->resolve();
		d->resolve();
	}

	r = c->write(MEM, w, 0x0000000000000);
	CHECK(r == OK);
	r = c->write(FETCH, w, 0x0000000000001);
	CHECK(r == WAIT);

	c->resolve();
	d->resolve();

	actual = d->view(0, 1)[0];
	// we do NOT write back now!
	REQUIRE(expected == actual);

	expected.at(0) = w;
	actual = c->view(0, 1)[0];
	REQUIRE(expected == actual);

	r = c->write(FETCH, w, 0x0000000000001);
	// this should have been loaded already!
	CHECK(r == OK);

	c->resolve();
	d->resolve();

	expected.at(1) = w;
	actual = c->view(0, 1)[0];
	REQUIRE(expected == actual);

	delete d;
	delete c;
}

TEST_CASE("dram takes \"forever\", two concurrent requests different index", "[cache]")
{
	int delay = 0;
	Dram *d = new Dram(MEM_SIZE, delay + 2);
	Cache *c = new Cache(d, delay);
	std::array<signed int, LINE_SIZE> expected = {0, 0, 0, 0};
	std::array<signed int, LINE_SIZE> actual = d->view(0, 1)[0];
	CHECK(expected == actual);

	signed int w = 0x11223344;

	int i;
	Response r;
	for (i = 0; i < delay + 2; ++i) {
		r = c->write(MEM, w, 0x0000000000000);
		CHECK(r == BLOCKED); // BLOCKED

		r = c->write(FETCH, w, 0x0000000000100);
		CHECK(r == WAIT); // WAIT

		actual = c->view(0, 1)[0];
		REQUIRE(expected == actual);
		c->resolve();
		d->resolve();
	}

	r = c->write(MEM, w, 0x0000000000000);
	CHECK(r == OK);
	r = c->write(FETCH, w, 0x0000000000001);
	CHECK(r == WAIT);

	c->resolve();
	d->resolve();

	actual = d->view(0, 1)[0];
	// we do NOT write back now!
	REQUIRE(expected == actual);

	expected.at(0) = w;
	actual = c->view(0, 1)[0];
	REQUIRE(expected == actual);

	for (i = 0; i < delay + 2; ++i) {
		r = c->write(FETCH, w, 0x0000000000100);
		CHECK(r == BLOCKED); // WAIT

		actual = c->view(0, 1)[0];
		REQUIRE(expected == actual);
		c->resolve();
		d->resolve();
	}

	r = c->write(FETCH, w, 0x0000000000001);
	CHECK(r == OK);

	c->resolve();
	d->resolve();

	expected.at(1) = w;
	actual = c->view(0, 1)[0];
	REQUIRE(expected == actual);

	delete d;
	delete c;
}

TEST_CASE("dram takes \"forever\", two concurrent requests different tag", "[cache]")
{
	// TODO
}
