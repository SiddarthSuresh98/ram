#include "storage.h"
#include "definitions.h"
#include <algorithm>

Storage::Storage(int delay) {
	this->data = new std::vector<std::array<signed int, LINE_SIZE>>;
	this->delay = delay;
	this->lower = nullptr;
	this->current_request = nullptr;
	this->wait_time = this->delay;
}

std::vector<std::array<signed int, LINE_SIZE>>
Storage::view(int base, int lines) const
{
	base = (base / LINE_SIZE) * LINE_SIZE;
	std::vector<std::array<signed int, LINE_SIZE>> ret(lines + 1);
	std::copy(this->data->begin() + base, this->data->begin() + base + lines, ret.begin());
	return ret;
}
