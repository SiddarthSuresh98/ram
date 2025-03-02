#define CATCH_CONFIG_MAIN
#include "fact.h"
#include<catch2/catch_test_macros.hpp>

TEST_CASE( "factorials are computed", "[factorial]")
{
    REQUIRE( factorial(1) == 1 );
    REQUIRE( factorial(2) == 2 );
    REQUIRE( factorial(3) == 6 );
    REQUIRE( factorial(10) == 3628800 );
}
