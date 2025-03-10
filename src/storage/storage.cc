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

void Storage::resolve()
{
	if (this->wait_time == 0) {
		this->requester = IDLE;
		this->wait_time = delay;
	} else if (this->requester != IDLE && !this->is_waiting) {
		--this->wait_time;
	}
}
