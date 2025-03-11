#include "dram.h"
#include "definitions.h"
#include "response.h"
#include <algorithm>
#include <bits/stdc++.h>
#include <bitset>
#include <iostream>
#include <iterator>

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

void Dram::do_write(signed int data, int address)
{
	int line = address / LINE_SIZE;
	int word = address % LINE_SIZE;

	this->data->at(line).at(word) = data;
}

void Dram::do_write_line(std::array<signed int, LINE_SIZE> data_line, int address){
	int line = address / LINE_SIZE;
	this->data->at(line) = data_line;
}

void Dram::do_read(std::array<signed int, LINE_SIZE> &data_line, int address)
{
	int line = address / LINE_SIZE;
	data_line = this->data->at(line);
}

void Dram::do_read_word(signed int &data, int address)
{
	int line = address / LINE_SIZE;
	int word = address % LINE_SIZE;
	data = this->data->at(line).at(word);
}



Response Dram::write_line(Accessor accessor, std::array<signed int, LINE_SIZE> data_line, int address)
{
	Response r = WAIT;

	if (accessor == SIDE) {
		this->do_write_line(data_line, address);
		r = OK;
	} else {
		/* Do this first--then process the first cycle immediately. */
		if (this->requester == IDLE)
			this->requester = accessor;

		if (this->requester == accessor) {
			if (this->wait_time == 0) {
				this->do_write_line(data_line, address);
				r = OK;
			}
		}
	}
	return r;
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

Response Dram::read(Accessor accessor, int address, std::array<signed int, LINE_SIZE>& data) { 
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

Response Dram::read_word(Accessor accessor, int address, signed int& data) { 
	Response r = WAIT;

	if (this->requester == IDLE)
		this->requester = accessor;

	if (this->requester == accessor) {
		if (this->wait_time == 0) {
			this->do_read_word(data, address);
			r = OK;
		}
	}

	return r;
}

std::ostream &operator<<(std::ostream &os, const Dram &d)
{
	const auto default_flags = std::cout.flags();
	const auto default_fill = std::cout.fill();

	std::vector<std::array<signed int, LINE_SIZE>> data = d.view(0, MEM_SIZE);

	os << " " << std::setfill(' ') << std::setw(MEM_SPEC + 2) << "INDEX"
	   << " | " << std::setfill(' ') << std::setw((8 + 3) * 4 - 1) << "DATA"
	   << std::endl;
	for (int i = 0; i < MEM_SIZE; ++i) {
		os << " 0b" << std::setw(MEM_SPEC) << std::bitset<MEM_SPEC>(i) << " | ";
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

