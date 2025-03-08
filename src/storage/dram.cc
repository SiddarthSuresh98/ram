#include "dram.h"
#include "definitions.h"
#include "response.h"
#include <algorithm>

Dram::Dram(int lines, int delay)
{
	this->data = new std::vector<std::array<signed int, LINE_SIZE>>;
	this->data->resize(lines);
	this->delay = delay;
	this->wait_time = this->delay;
	this->lower = nullptr;
}

Dram::~Dram() { delete this->data; }

Response Dram::write(Accessor accessor, signed int data, int address)
{
	Response r = WAIT;

	if (accessor == SIDE) {
		this->do_write(data, address);
		r = OK;
	} else {
		/* Do this first--then process the first cycle immediately. */
		this->deque.push_back(accessor);

		if (this->deque.front() == accessor) {
			if (this->wait_time == 0) {
				this->do_write(data, address);
				r = OK;
			}
		}
	}

	return r;
}

Response Dram::read(Accessor accessor, int address) { return WAIT; }
