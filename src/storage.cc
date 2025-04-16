#include "storage.h"
#include "definitions.h"
#include <algorithm>
#include <stdexcept>

Storage::Storage(int delay)
{
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

int
Storage::preprocess(void *id)
{
	if (id == nullptr)
		throw std::invalid_argument("Accessor cannot be nullptr.");

	if (this->current_request == nullptr)
		this->current_request = id;

	return this->current_request == id;
}

int
Storage::is_data_ready()
{
	int r;

	r = 0;
	if (this->wait_time == 0) {
		this->current_request = nullptr;
		this->wait_time = delay;
		r = 1;
	} else {
		--this->wait_time;
	}

	return r;
}
