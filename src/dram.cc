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
	if (!preprocess(id) || !this->is_data_ready())
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
