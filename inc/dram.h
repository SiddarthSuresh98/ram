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

#ifndef DRAM_H
#define DRAM_H
#include "definitions.h"
#include "storage.h"
#include <functional>
#include <ostream>

class Dram : public Storage
{
  public:
	/**
	 * Constructor.
	 * @param The number of clock cycles each access takes.
	 * @return A new memory object.
	 */
	Dram(int delay);
	~Dram();

	int write_word(void *, signed int, int) override;
	int write_line(void *, std::array<signed int, LINE_SIZE>, int) override;
	int read_word(void *, int, signed int &) override;
	int read_line(void *, int, std::array<signed int, LINE_SIZE> &) override;

	/**
	 * TODO This will accept a file at a later date.
	 */
	void load(std::vector<signed int> program);

  private:
	int process(void *id, int address, std::function<void(int line, int word)> request_handler) override;
	/**
	 * Given `address`, returns the line and word it is in.
	 * @param an address
	 * @param the line (row) `address` is in
	 * @param the word (column) `address` corresponds to
	 */
	void get_memory_index(int address, int &line, int &word);
};

#endif /* DRAM_H_INCLUDED */
