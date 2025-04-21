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

#ifndef STORAGE_H
#define STORAGE_H
#include "definitions.h"
#include <algorithm>
#include <array>
#include <functional>
#include <map>
#include <vector>

// clang-format off
/**
 * Ensures address is within the current memory size using a clean wrap.
 * @param an address
 */
#define WRAP_ADDRESS(a) \
  ((a < 0) ? ((a % MEM_WORDS) + MEM_WORDS) % MEM_WORDS : a % MEM_WORDS)
// clang-format on

class Storage
{
  public:
	/**
	 * Constructor.
	 * @param The time an access to this storage device takes.
	 * @return A newly allocated storage object.
	 */
	Storage(int delay);
	virtual ~Storage() = default;

	/**
	 * Write `data` word into `address`.
	 * @param the source making the request.
	 * @param the data (hexadecimal) to write.
	 * @param the address to write to.
	 * @return 1 if the request was completed, 0 otherwise.
	 */
	virtual int write_word(void *id, signed int data, int address) = 0;
	virtual int write_line(void *id, std::array<signed int, LINE_SIZE> data_line, int address) = 0;

	/**
	 * Get the data line at `address`.
	 * @param the source making the request.
	 * @param the address being accessed.
	 * @param the data being returned
	 * @return 1 if the request was completed, 0 otherwise
	 */
	virtual int read_line(void *id, int address, std::array<signed int, LINE_SIZE> &data) = 0;
	virtual int read_word(void *id, int address, signed int &data) = 0;

	/**
	 * Sidedoor view of `lines` of memory starting at `base`.
	 * @param The base line to start getting memory from.
	 * @param The amount of lines to fetch.
	 * @return A matrix of data values, where each row is a line and each column
	 * is a word.
	 */
	std::vector<std::array<signed int, LINE_SIZE>> view(int base, int lines) const;

  protected:
	/**
	 * Helper for all access methods.
	 * Calls `request_handler` when `id` is allowed to complete its
	 * request cycle.
	 * May include extra checks depending on storage device.
	 * @param the source making the request
	 * @param the address to write to
	 * @param the function to call when an access should be completed
	 */
	virtual int
	process(void *id, int address, std::function<void(int index, int offset)> request_handler) = 0;
	/**
	 * Helper for process. Given `id`, returns 0 if the request should trivially be ignored.
	 * @param the source making the request
	 * @return 0 if the request should not be completed, 1 if it should be evaluated further.
	 */
	int preprocess(void *id);
	/**
	 * Returns OK if `id` should complete its request this cycle. In the case it can, automatically
	 * clears the current requester.
	 * @param the id asking for a resource
	 * @return 1 if the access can be carried out this function call, 0 otherwise.
	 */
	int is_data_ready();
	/**
	 * The data currently stored in this level of storage.
	 */
	std::vector<std::array<signed int, LINE_SIZE>> *data;
	/**
	 * A pointer to the next lowest level of storage.
	 * Used in case of cache misses.
	 */
	Storage *lower;
	/**
	 * The id currently being serviced.
	 */
	void *current_request;
	/**
	 * The number of clock cycles this level of storage takes to complete
	 * requests.
	 */
	int delay;
	/**
	 * The number of cycles until the current request is completed.
	 */
	int wait_time;
};

#endif /* STORAGE_H_INCLUDED */
