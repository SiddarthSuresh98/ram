#include "dram.h"
#include "definitions.h"
#include "response.h"
#include <algorithm>
#include <bits/stdc++.h>
#include <bitset>
#include <iostream>
#include <iterator>
#include <utils.h>

Dram::Dram(int delay)
{
	this->data = new std::vector<std::array<signed int, LINE_SIZE>>;
	this->data->resize(MEM_LINES);
	this->delay = delay;
	this->is_waiting = false;
	this->lower = nullptr;
	this->requester = IDLE;
	this->wait_time = this->delay;
}

Dram::~Dram() { delete this->data; }

Response Dram::write_line(
	Accessor accessor, std::array<signed int, LINE_SIZE> data_line, int address)
{
	return process(accessor, address, [&](int line, int word) {
		(void)word;
		this->data->at(line) = data_line;
	});
}

Response Dram::write_word(Accessor accessor, signed int data, int address)
{
	return process(accessor, address, [&](int line, int word) {
		this->data->at(line).at(word) = data;
	});
}

// TODO requires testing
Response Dram::read_line(
	Accessor accessor,
	int address,
	std::array<signed int, LINE_SIZE> &data_line)
{
	return process(accessor, address, [&](int line, int word) {
		(void)word;
		data_line = this->data->at(line);
	});
}

Response Dram::read_word(Accessor accessor, int address, signed int &data)
{
	return process(accessor, address, [&](int line, int word) {
		data = this->data->at(line).at(word);
	});
}

// TODO load a file instead and test this method
void Dram::load(std::vector<signed int> program) {
	unsigned long i;
	for (i = 0; i < program.size(); ++i) {
		int line, word;
		get_memory_index(i, line, word);
		this->data->at(line).at(word) = program[i];
	}
}

Response Dram::process(
	Accessor accessor,
	int address,
	std::function<void(int line, int word)> request_handler)
{
	Response r = this->is_access_cleared(accessor);
	if (r == OK) {
		int line, word;
		get_memory_index(address, line, word);
		request_handler(line, word);
	}
	return r;
}

Response Dram::is_access_cleared(Accessor accessor)
{
	Response r;
	r = WAIT;
	/* Do this first--then process the first cycle immediately. */
	if (accessor == SIDE)
		r = OK;
	else {
		if (this->requester == IDLE)
			this->requester = accessor;
		if (this->requester == accessor) {
			if (this->wait_time == 0) {
				this->requester = IDLE;
				this->wait_time = delay;
				r = OK;
			} else {
				--this->wait_time;
			}
		}
	}
	return r;
}

std::ostream &operator<<(std::ostream &os, const Dram &d)
{
	const auto default_flags = std::cout.flags();
	const auto default_fill = std::cout.fill();

	std::vector<std::array<signed int, LINE_SIZE>> data = d.view(0, MEM_LINES);

	os << " " << std::setfill(' ') << std::setw(MEM_LINE_SPEC + 2 + LINE_SPEC)
	   << "ADDRESS"
	   << " | " << std::setfill(' ') << std::setw((8 + 3) * 4 - 1) << "DATA"
	   << std::endl;
	for (int i = 0; i < MEM_LINES; ++i) {
		os << " 0b" << std::setw(MEM_LINE_SPEC + LINE_SPEC) << left
		   << std::bitset<MEM_LINE_SPEC>(i) << " | ";
		for (int j = 0; j < LINE_SIZE; ++j) {
			os << "0x" << std::setfill('0') << std::setw(8) << std::hex
			   << data.at(i).at(j) << ' ';
		}
		os << std::endl;
	}

	std::cout.flags(default_flags);
	std::cout.fill(default_fill);
	return os;
}
