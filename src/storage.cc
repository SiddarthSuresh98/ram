#include "storage.h"
#include "definitions.h"
#include <algorithm>

std::vector<std::array<signed int, LINE_SIZE>>
Storage::view(int base, int lines) const
{
	base = (base / LINE_SIZE) * LINE_SIZE;
	std::vector<std::array<signed int, LINE_SIZE>> ret(lines + 1);
	std::copy(
		this->data->begin() + base, this->data->begin() + base + lines,
		ret.begin());
	return ret;
}

Storage *Storage::get_lower() { return this->lower; }
