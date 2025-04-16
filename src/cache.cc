#include "cache.h"
#include "definitions.h"
#include <cstdlib>
#include <iostream>
#include <iterator>

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
}

Cache::~Cache()
{
	delete this->lower;
	delete this->data;
}

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
	if (!preprocess(id) || priming_address(address) || !this->is_data_ready())
		return 0;

	int tag, index, offset;
	GET_FIELDS(address, &tag, &index, &offset);
	index = this->get_true_index(index);
	request_handler(index, offset);

	return 1;
}

int
Cache::priming_address(int address)
{
	unsigned int tag, index, offset;
	int r1, r2;
	std::array<signed int, LINE_SIZE> *evict;
	std::array<int, 3> *meta;

	r1 = 0;
	GET_FIELDS(address, &tag, &index, &offset);
	index = this->get_true_index(index);

	if (this->is_address_missing(index, tag)) {
		r1 = 1;

		index = this->get_replacement_index(index);
		meta = &this->meta.at(index);
		evict = &this->data->at(index);

		// handle eviction of dirty cache lines
		if (meta->at(1) >= 0) {
			r2 = this->lower->write_line(
				this, *evict, ((index << LINE_SPEC) + (meta->at(0) << (this->size + LINE_SPEC))));
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

unsigned int
Cache::is_address_missing(unsigned int index, unsigned int tag)
{
	int i;

	for (i = 0; i < (1 << this->ways); ++i)
		if (this->meta.at(index + i).at(0) == tag)
			return i;
	return -1;
}

unsigned int
Cache::get_true_index(unsigned int index)
{
	return index * (1 << this->ways);
}

unsigned int
Cache::get_replacement_index(unsigned int index)
{
	return index + (rand() % (1 << this->ways));
}
