#include "accessor.h"
#include <iostream>

std::ostream &operator<<(std::ostream &os, Accessor a)
{
	const std::string nameA[] = {
		"IDLE", "WRITE", "MEM", "EXEC", "DCDE", "FETCH", "L1CACHE", "SIDE",
	};
	return os << nameA[a];
}
