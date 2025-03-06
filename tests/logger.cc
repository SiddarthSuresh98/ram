#include "logger.h"
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <regex>
#include <sstream>

TEST_CASE("Logger logs higher log level", "[logger]")
{
	std::streambuf *coutBuffer = std::cout.rdbuf();
	std::ostringstream oss;
	std::cout.rdbuf(oss.rdbuf());

	Logger logger("");
	logger.setLevel(INFO);

	logger.log(ERROR, "foo bar baz qux");

	std::cout.rdbuf(coutBuffer);

	std::string actual = oss.str();
	std::regex expected(
		"\\[\\d{4}-\\d{2}-\\d{2} \\d{2}:\\d{2}:\\d{2}\\] ERROR: "
		"foo bar baz qux\\n");

	REQUIRE(std::regex_match(actual, expected));
}

TEST_CASE("Logger logs equal log level", "[logger]")
{
	std::streambuf *coutBuffer = std::cout.rdbuf();
	std::ostringstream oss;
	std::cout.rdbuf(oss.rdbuf());

	Logger logger("");
	logger.setLevel(INFO);

	logger.log(INFO, "foo bar baz qux");

	std::cout.rdbuf(coutBuffer);

	std::string actual = oss.str();
	std::regex expected("\\[\\d{4}-\\d{2}-\\d{2} \\d{2}:\\d{2}:\\d{2}\\] INFO: "
						"foo bar baz qux\\n");

	REQUIRE(std::regex_match(actual, expected));
}

TEST_CASE("Logger ignores lower log level", "[logger]")
{
	std::streambuf *coutBuffer = std::cout.rdbuf();
	std::ostringstream oss;
	std::cout.rdbuf(oss.rdbuf());

	Logger logger("");
	logger.setLevel(INFO);

	logger.log(DEBUG, "foo bar baz qux");

	std::cout.rdbuf(coutBuffer);

	std::string actual = oss.str();
	std::string expected("");

	REQUIRE(actual == expected);
}
