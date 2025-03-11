#include "cache.h"
#include "definitions.h"
#include "response.h"
#include "utils.h"
#include <bits/stdc++.h>
#include <bitset>
#include <iostream>
#include <iterator>

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

Cache::~Cache()
{
	delete this->lower;
	delete this->data;
}

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
			this->meta[index].at(1) = 1;
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
	std::array<int, 2> *meta;

	get_bit_fields(expected, &tag, &index, &offset);
	meta = &this->meta.at(index);

	if (meta->at(0) != tag) {
		// address not in cache
		if (meta->at(1) >= 0) {
			// occupant is dirty
			// TODO
			r = WAIT;
		} else {
			actual = this->data->at(index);
			r = this->lower->read(L1CACHE, expected, actual);
			if (r == OK) {
				meta->at(0) = tag;
				meta->at(1) = -1;
			}
		}
	}

	this->is_waiting = (r == OK) ? false : true;
}

std::array<std::array<int, 2>, L1_CACHE_SIZE> Cache::get_meta() const
{
	std::array<std::array<int, 2>, L1_CACHE_SIZE> ret;
	std::copy(std::begin(this->meta), std::end(this->meta), std::begin(ret));
	return ret;
}

std::ostream &operator<<(std::ostream &os, const Cache &c)
{
	const auto default_flags = std::cout.flags();
	const auto default_fill = std::cout.fill();

	std::vector<std::array<signed int, LINE_SIZE>> data =
		c.view(0, L1_CACHE_SIZE);
	std::array<std::array<int, 2>, L1_CACHE_SIZE> meta = c.get_meta();

	cout << data.capacity();
	os << " " << std::setfill(' ') << std::setw(L1_CACHE_SPEC + 2) << "INDEX"
	   << " | " << std::setfill(' ') << std::setw((8 + 3) * 4 - 1) << "DATA"
	   << " | " << std::setfill(' ')
	   << std::setw(MEM_SPEC - LINE_SPEC - L1_CACHE_SPEC + 2) << "TAG"
	   << " | D\n";
	for (int i = 0; i < L1_CACHE_SIZE; ++i) {
		os << " 0b" << std::setw(L1_CACHE_SPEC) << std::bitset<L1_CACHE_SPEC>(i)
		   << " | ";
		for (int j = 0; j < LINE_SIZE; ++j) {
			os << "0x" << std::setfill('0') << std::setw(8) << std::hex
			   << data.at(i).at(j) << " ";
		}
		os << "| 0x" << std::setfill(' ')
		   << std::bitset<MEM_SPEC - LINE_SPEC - L1_CACHE_SPEC>(meta.at(i)[0])
		   << " | " << (int)(meta.at(i)[0] >= 0) << '\n';
	}

	std::cout.flags(default_flags);
	std::cout.fill(default_fill);
	return os;
}
