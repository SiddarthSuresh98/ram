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

// TEST_CASE("no delay stores instantly", "[cache]")
// {
// 	int delay = 0;
// 	Dram *d = new Dram(MEM_SIZE, delay);
// 	Cache *c = new Cache(d, delay);
// 	std::array<signed int, LINE_SIZE> expected = {0, 0, 0, 0};
// 	std::array<signed int, LINE_SIZE> actual = d->view(0, 1)[0];
// 	CHECK(expected == actual);

// 	signed int w = 0x11223344;

// 	Response r;
	
// 	r = c->write(MEM, w, 0x0000000000000);	
// 	CHECK(r == OK);
// 	d->resolve();
// 	c->resolve();

// 	expected.at(0) = w;
// 	actual = c->view(0, 1)[0];
// 	REQUIRE(expected == actual);

// 	actual = d->view(0, 1)[0];
// 	// we do NOT write back now!
// 	REQUIRE(expected != actual);

// 	delete d;
// 	delete c;
// }
