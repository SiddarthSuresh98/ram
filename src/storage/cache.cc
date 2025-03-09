#include "cache.h"
#include "definitions.h"
#include "response.h"
#include <bits/stdc++.h>

Cache::Cache(int lines, Storage *lower, int delay)
{
	this->data = new std::vector<std::array<signed int, LINE_SIZE>>;
	this->data->resize(L1_CACHE_SIZE);
	this->lower = lower;
	this->delay = delay;
	for (int i = 0; i < L1_CACHE_SIZE; ++i)
		this->stat[i] = 0b01;
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
		if (this->wait_time == 0) {
			// this->do_write(data, address);
			r = OK;
		}
	}

	return r;
}

Response Cache::read(Accessor accessor, int address, std::array<signed int, LINE_SIZE>& data) { return WAIT; }

void Cache::fetch_resource(int address) {

}
