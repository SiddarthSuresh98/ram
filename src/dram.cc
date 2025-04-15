#include "dram.h"
#include "definitions.h"
#include <algorithm>
#include <bits/stdc++.h>
#include <bitset>
#include <iterator>

Dram::Dram(int delay) : Storage(delay) { this->data->resize(MEM_LINES); }

Dram::~Dram() { delete this->data; }

int
Dram::write_line(void *id, std::array<signed int, LINE_SIZE> data_line, int address)
{
	return process(id, address, [&](int line, int word) {
		(void)word;
		this->data->at(line) = data_line;
	});
}

int
Dram::write_word(void *id, signed int data, int address)
{
	return process(id, address, [&](int line, int word) { this->data->at(line).at(word) = data; });
}

int
Dram::read_line(void *id, int address, std::array<signed int, LINE_SIZE> &data_line)
{
	return process(id, address, [&](int line, int word) {
		(void)word;
		data_line = this->data->at(line);
	});
}

int
Dram::read_word(void *id, int address, signed int &data)
{
	return process(id, address, [&](int line, int word) { data = this->data->at(line).at(word); });
}

// TODO load a file instead and test this method
void
Dram::load(std::vector<signed int> program)
{
	unsigned long i;
	for (i = 0; i < program.size(); ++i) {
		int line, word;
		get_memory_index(i, line, word);
		this->data->at(line).at(word) = program[i];
	}
}

int
Dram::process(void *id, int address, std::function<void(int line, int word)> request_handler)
{
	if (id == nullptr)
		throw std::invalid_argument("Accessor cannot be nullptr.");

	if (this->current_request == nullptr)
		this->current_request = id;

	if (this->current_request != id)
		return 0;

	if (!this->is_data_ready())
		return 0;

	int line, word;
	get_memory_index(address, line, word);
	request_handler(line, word);
	return 1;
}

void
Dram::get_memory_index(int address, int &line, int &word)
{
	line = WRAP_ADDRESS(address) / LINE_SIZE;
	word = address % LINE_SIZE;
}
