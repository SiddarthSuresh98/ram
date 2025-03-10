#include "accessor.h"
#include <iostream>

std::ostream &operator<<(std::ostream &os, Accessor a)
{
	const std::string nameA[] = {"IDLE", "MEM", "FETCH", "L1CACHE", "SIDE"};
	return os << nameA[a];
}
