#include "cache.h"
#include "definitions.h"
#include "response.h"
#include "utils.h"
#include <bits/stdc++.h>

Cache::Cache(int lines, Storage *lower, int delay)
{
	this->data = new std::vector<std::array<signed int, LINE_SIZE>>;
	this->data->resize(L1_CACHE_SIZE);
	this->lower = lower;
	this->delay = delay;
	this->meta.fill({-1});
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
		if (this->is_waiting == true)
			r = BLOCKED;
		else if (this->wait_time == 0) {
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
	int etag, index, atag;
	std::array<signed int, LINE_SIZE> actual;
	std::array<int, 2> meta;

	get_bit_fields(expected, &etag, &index, nullptr);
	meta = this->meta.at(index);

	if (atag != etag) {
		// address not in cache
		if (this->meta[index][0]) {
			// occupant is dirty
			// TODO
			r = WAIT;
		} else {
			actual = this->data->at(index);
			r = this->lower->read(L1CACHE, expected, actual);
		}
	}

	this->is_waiting = (r == OK) ? false : true;
}
