#include "response.h"
#include <iostream>

std::ostream &operator<<(std::ostream &os, Response r)
{
	const std::string nameR[] = {"OK", "WAIT", "BLOCKED", "STALLED"};
	return os << nameR[r];
}
