#include "cache.h"
#include "definitions.h"
#include <iostream>
#include <iterator>

Cache::Cache(Storage *lower, unsigned int size, int delay) : Storage(delay)
{
	int true_size;

	true_size = 1 << size;
	this->data->resize(true_size);
	this->meta = std::vector<std::array<signed int, 2>>(true_size, {-1, -1});
	this->size = size;
	this->lower = lower;
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
	if (id == nullptr)
		throw std::invalid_argument("Accessor cannot be nullptr.");

	if (this->current_request == nullptr)
		this->current_request = id;

	if (this->current_request != id)
		return 0;

	if (is_address_missing(address))
		return 0;

	if (!this->is_data_ready())
		return 0;

	int tag, index, offset;
	GET_FIELDS(address, &tag, &index, &offset);
	request_handler(index, offset);

	return 1;
}

int
Cache::is_address_missing(int expected)
{
	int r, q, tag, index, offset;
	std::array<signed int, LINE_SIZE> *actual;
	std::array<int, 2> *meta;

	GET_FIELDS(expected, &tag, &index, &offset);
	r = 0;
	meta = &this->meta.at(index);
	actual = &this->data->at(index);

	if (meta->at(0) != tag) {
		r = 1;
		if (meta->at(1) >= 0) {
			q = this->lower->write_line(
				this, *actual, ((index << LINE_SPEC) + (meta->at(0) << (this->size + LINE_SPEC))));
			if (q) {
				meta->at(1) = -1;
			}
		} else {
			q = this->lower->read_line(this, expected, *actual);
			if (q) {
				meta->at(0) = tag;
			}
		}
	}

	return r;
}
