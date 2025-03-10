#include "dram.h"
#include "definitions.h"
#include "response.h"
#include <algorithm>

Dram::Dram(int lines, int delay)
{
	this->data = new std::vector<std::array<signed int, LINE_SIZE>>;
	this->data->resize(lines);
	this->delay = delay;
	this->is_waiting = false;
	this->lower = nullptr;
	this->requester = IDLE;
	this->wait_time = this->delay;
}

Dram::~Dram() { delete this->data; }

void Dram::do_write(signed data, int address)
{
	int line = address / LINE_SIZE;
	int word = address % LINE_SIZE;

	this->data->at(line).at(word) = data;
}

Response Dram::write(Accessor accessor, signed int data, int address)
{
	Response r = WAIT;

	if (accessor == SIDE) {
		this->do_write(data, address);
		r = OK;
	} else {
		/* Do this first--then process the first cycle immediately. */
		if (this->requester == IDLE)
			this->requester = accessor;

		if (this->requester == accessor) {
			if (this->wait_time == 0) {
				this->do_write(data, address);
				r = OK;
			}
		}
	}

	return r;
}

void Dram::do_read(std::array<signed int, LINE_SIZE> &data_line, int address)
{
	int line = address / LINE_SIZE;
	data_line = this->data->at(line);
}

Response Dram::read(
	Accessor accessor, int address, std::array<signed int, LINE_SIZE> &data)
{
	Response r = WAIT;
	if (this->requester == IDLE)
		this->requester = accessor;
	if (this->requester == accessor) {
		if (this->wait_time == 0) {
			this->do_read(data, address);
			r = OK;
		}
	}
	return r;
}
