#include "cache.h"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Constructor initialize test 1", "[cache]")
{
	Cache *c = new Cache(1, nullptr, 4);
	delete c;
}
