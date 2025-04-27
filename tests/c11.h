#ifndef C11_H
#define C11_H

#include "cache.h"
#include "dram.h"
#include "storage.h"
#include <algorithm>
#include <array>
#include <catch2/catch_test_macros.hpp>
#include <functional>

/**
 * one way associative, single level
 */
class C11
{
  public:
	C11()
	{
		this->m_delay = 4;
		this->c_delay = 2;
		this->mem = new int();
		this->fetch = new int();
		this->c = new Cache(new Dram(this->m_delay), 5, 0, this->c_delay);
		this->expected = {0, 0, 0, 0};
		this->actual = this->c->get_data()[0];
	}

	~C11()
	{
		delete this->c;
		delete this->mem;
		delete this->fetch;
	}

	void
	wait_then_do(int delay, std::function<int()> f)
	{
		for (int i = 0; i < delay; ++i) {
			int r = f();

			// check response
			CHECK(!r);
			// check for early modifications
			actual = c->get_data()[0];
			REQUIRE(this->expected == this->actual);
		}
	}

	int m_delay;
	int c_delay;
	Cache *c;
	int *mem;
	int *fetch;
	std::array<signed int, LINE_SIZE> expected;
	std::array<signed int, LINE_SIZE> actual;
};

#endif /* C11_H_INCLUDED */
