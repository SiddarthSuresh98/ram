#include "cache.h"
#include "definitions.h"
#include "utils.h"
#include <bits/stdc++.h>
#include <iterator>

Cache::Cache(Storage *lower, int delay) : Storage(delay)
{
	this->data->resize(L1_CACHE_LINES);
	this->lower = lower;
	this->meta.fill({-1, -1});
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

// TODO: tests for multi level cache
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
	int r;
	r = this->is_access_cleared(id, address);
	if (r) {
		int tag, index, offset;
		get_cache_fields(address, &tag, &index, &offset);
		request_handler(index, offset);
	}
	return r;
}

int
Cache::is_access_cleared(void *id, int address)
{
	/* Do this first--then process the first cycle immediately. */
	if (id == nullptr)
		throw std::invalid_argument("Accessor cannot be nullptr.");
	if (this->current_request == nullptr)
		this->current_request = id;
	if (this->current_request == id) {
		if (is_address_missing(address))
			return 0;
		else if (this->wait_time == 0) {
			this->current_request = nullptr;
			this->wait_time = delay;
			return 1;
		} else {
			--this->wait_time;
		}
	}
	return 0;
}

int
Cache::is_address_missing(int expected)
{
	int r, q, tag, index, offset;
	std::array<signed int, LINE_SIZE> *actual;
	std::array<int, 2> *meta;

	get_cache_fields(expected, &tag, &index, &offset);
	r = 0;
	meta = &this->meta.at(index);
	actual = &this->data->at(index);

	if (meta->at(0) != tag) {
		r = 1;
		if (meta->at(1) >= 0) {
			q = this->lower->write_line(
				this, *actual,
				((index << LINE_SPEC) + (meta->at(0) << (L1_CACHE_LINE_SPEC + LINE_SPEC))));
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

std::array<std::array<int, 2>, L1_CACHE_LINES>
Cache::get_meta() const
{
	std::array<std::array<int, 2>, L1_CACHE_LINES> ret;
	std::copy(std::begin(this->meta), std::end(this->meta), std::begin(ret));
	return ret;
}
