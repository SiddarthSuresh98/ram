// Memory subsystem for the RISC-V[ECTOR] mini-ISA
// Copyright (C) 2025 Siddarth Suresh
// Copyright (C) 2025 bdunahu

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "cache.h"
#include "definitions.h"
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <limits.h>

Cache::Cache(Storage *lower, unsigned int size, unsigned int ways, int delay) : Storage(delay)
{
	int true_size;

	true_size = 1 << size;
	this->data->resize(true_size);
	this->meta = std::vector<std::array<signed int, 3>>(true_size, {-1, -1, -1});
	this->lower = lower;

	this->size = size;
	// store the number of bits which are moved into the tag field
	this->ways = ways;
	this->access_num = 0;
}

Cache::~Cache()
{
	delete this->lower;
	delete this->data;
}

unsigned int
Cache::get_size() { return this->size; }

int
Cache::write_word(void *id, signed int data, int address)
{
	return process(id, address, [&](int index, int offset) {
		this->data->at(index).at(offset) = data;
		this->meta[index].at(1) = 1;
	});
}

int
Cache::write_line(void *id, std::array<signed int, LINE_SIZE> data_line, int address)
{
	return process(id, address, [&](int index, int offset) {
		(void)offset;
		this->data->at(index) = data_line;
		this->meta[index].at(1) = 1;
	});
}

int
Cache::read_line(void *id, int address, std::array<signed int, LINE_SIZE> &data_line)
{
	return process(id, address, [&](int index, int offset) {
		(void)offset;
		data_line = this->data->at(index);
	});
}

int
Cache::read_word(void *id, int address, signed int &data)
{
	return process(
		id, address, [&](int index, int offset) { data = this->data->at(index).at(offset); });
}

int
Cache::process(void *id, int address, std::function<void(int index, int offset)> request_handler)
{
	address = WRAP_ADDRESS(address);
	if (!preprocess(id) || priming_address(address) || !this->is_data_ready())
		return 0;

	int tag, index, offset;
	std::array<int, 3> *meta;

	GET_FIELDS(address, &tag, &index, &offset);
	index = this->search_ways_for(index, tag);
	request_handler(index, offset);
	// set usage status
	meta = &this->meta.at(index);
	meta->at(2) = (this->access_num % INT_MAX);
	++this->access_num;

	return 1;
}

int
Cache::priming_address(int address)
{
	int tag, index, offset, t_index;
	int r1, r2;
	std::array<signed int, LINE_SIZE> *evict;
	std::array<int, 3> *meta;

	r1 = 0;
	GET_FIELDS(address, &tag, &index, &offset);
	t_index = this->search_ways_for(index, tag);
	meta = &this->meta.at(t_index);

	if (meta->at(0) != tag) {
		r1 = 1;

		evict = &this->data->at(t_index);

		// handle eviction of dirty cache lines
		if (meta->at(1) >= 0) {
			r2 = this->lower->write_line(
				this, *evict, ((index << LINE_SPEC) + (meta->at(0) << (this->size - this->ways + LINE_SPEC))));
			if (r2)
				meta->at(1) = -1;
		} else {
			r2 = this->lower->read_line(this, address, *evict);
			if (r2) {
				meta->at(0) = tag;
			}
		}
	}

	return r1;
}

int
Cache::search_ways_for(int index, int tag)
{
	int i, r;

	index = index * (1 << this->ways);
	r = INT_MAX;

	for (i = 0; i < (1 << this->ways); ++i)
		if (this->meta.at(index + i).at(0) == tag)
			return i + index;
	for (i = 0; i < (1 << this->ways); ++i)
		if (this->meta.at(index + i).at(2) < r)
			r = i;
	return r + index;
}
