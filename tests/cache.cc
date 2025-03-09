#include "cache.h"
#include "definitions.h"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Constructor singleton dram", "[cache]")
{
	Cache *c = new Cache(1, nullptr, LINE_SIZE);
	std::array<signed int, LINE_SIZE> expected = {0, 0, 0, 0};
	std::array<signed int, LINE_SIZE> actual = c->view(0, 1)[0];
	REQUIRE(expected == actual);
	delete c;
}
