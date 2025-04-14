#include "c11.h"
#include "cache.h"
#include "dram.h"
#include "storage.h"
#include <catch2/catch_test_macros.hpp>

/**
 * one way associative, two level
 */
class C21 : public C11
{
  public:
	C21() : C11()
	{
		Storage *s;

		s = new Dram(this->m_delay);
		s = new Cache(s, 5, this->c_delay);
		this->c = new Cache(s, 7, this->c_delay);
	}
};
