#include "dram.h"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Constructor initialize test 1", "[dram]")
{
	Dram *d = new Dram(1, 4);
	delete d;
}
