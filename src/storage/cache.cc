#include "cache.h"
#include "definitions.h"
#include "response.h"
#include "utils.h"
#include <bits/stdc++.h>

Cache::Cache(Storage *lower, int delay)
{
	this->data = new std::vector<std::array<signed int, LINE_SIZE>>;
	this->data->resize(L1_CACHE_SIZE);
	this->delay = delay;
	this->is_waiting = false;
	this->lower = lower;
	this->meta.fill({-1, -1});
	this->requester = IDLE;
	this->wait_time = this->delay;
}

Cache::~Cache() { delete this->data; }

Response Cache::write(Accessor accessor, signed int data, int address)
{
	Response r = WAIT;

	/* Do this first--then process the first cycle immediately. */
	if (this->requester == IDLE)
		this->requester = accessor;

	if (this->requester == accessor) {
		fetch_resource(address);
		if (this->is_waiting)
			r = BLOCKED;
		else if (this->wait_time == 0) {
			int tag, index, offset;
			get_bit_fields(address, &tag, &index, &offset);
			this->data->at(index).at(offset) = data;
			r = OK;
		}
	}

	return r;
}

Response Cache::read(
	Accessor accessor, int address, std::array<signed int, LINE_SIZE> &data)
{
	return WAIT;
}

void Cache::fetch_resource(int expected)
{
	Response r = OK;
	int tag, index, offset;
	std::array<signed int, LINE_SIZE> actual;
	std::array<int, 2> meta;

	get_bit_fields(expected, &tag, &index, &offset);
	meta = this->meta.at(index);

	if (this->meta[index][0] != tag) {
		// address not in cache
		if (this->meta[index][1] >= 0) {
			// occupant is dirty
			// TODO
			r = WAIT;
		} else {
			actual = this->data->at(index);
			r = this->lower->read(L1CACHE, expected, actual);
			// clear dirty bit and set tag?
			
		}
	}

	this->is_waiting = (r == OK) ? false : true;
}
