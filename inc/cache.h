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

#ifndef CACHE_H
#define CACHE_H
#include "definitions.h"
#include "storage.h"
#include <array>
#include <cmath>
#include <functional>
#include <ostream>

/**
 * Parse an address into a tag, index into the cache table, and a line
 * offset.
 * @param the address to be parsed
 * @param the resulting tag
 * @param the resulting index
 * @param the resulting offset
 */
// clang-format off
#define GET_FIELDS(a, t, i, o) \
    *(t) = GET_MID_BITS(a, this->size + LINE_SPEC - this->ways, MEM_WORD_SPEC); \
    *(i) = GET_MID_BITS(a, LINE_SPEC, this->size + LINE_SPEC - this->ways); \
    *(o) = GET_LS_BITS(a, LINE_SPEC)
// clang-format on

class Cache : public Storage
{
  public:
	/**
nn	 * Constructor.
	 * @param The number of `lines` contained in memory. The total number of
	 * words is this number multiplied by LINE_SIZE.
	 * @param The next lowest level in storage. Methods from this object are
	 * called in case of a cache miss.
	 * @param The number of bits required to specify a line in this level of cache.
	 * @param The number of ways this line of cache uses, or the number of data addresses stored for
   * certain address index.
	 * @param The number of clock cycles each access takes.
	 * @return A new cache object.
	 */
	Cache(Storage *lower, unsigned int size, unsigned int ways, int delay);
	~Cache();

	int write_word(void *, signed int, int) override;
	int write_line(void *, std::array<signed int, LINE_SIZE>, int) override;
	int read_line(void *, int, std::array<signed int, LINE_SIZE> &) override;
	int read_word(void *, int, signed int &) override;

  private:
	int process(
		void *id, int address, std::function<void(int index, int offset)> request_handler) override;
	/**
	 * Helper for process.
	 * Fetches `address` from a lower level of storage if it is not already
	 * present. The victim line is chosen/written back.
	 * @param the address that must be present in cache.
	 * @param 0 if the address is currently in cache, 1 if it is being fetched.
	 */
	int priming_address(int address);
	/**
	 * Walks the ways in this set of ways. If none of the tags match, returns -1. Otherwise, returns
	 * the index the matching data is located.
	 * @param an index aligned to the set of ways in `this->data'
	 * @param the tag to be matched
	 * @return -1 if the tag is not present in this set of ways (not in cache), or the true index if
	 * the tag is present.
	 */
	int is_address_missing(int true_index, int tag);
	/**
	 * Converts an index into a set of ways into an index into `this->data', which is a
	 * 1D array. The next `this->ways' entries after the returned index represent the ways in the
	 * set for this index.
	 * @param an index to a set of ways
	 * @param an index aligned to the set of ways in `this->data'
	 */
	int get_true_index(int index);
	/**
	 * Selects an index into the `data' and `meta' tables for write back using a random replacement
	 * policy.
	 * @param an index aligned to the set of ways in `this->data'
	 * @return an index aligned to the data line selected for eviction
	 */
	int get_replacement_index(int index);
	/**
	 * The number of bits required to specify a line in this level of cache.
	 */
	unsigned int size;
	/**
	 * The number of bits required to specify a way, or the number of data addresses stored for
	 * certain address index.
	 */
	unsigned int ways;
	/**
	 * An array of metadata about elements in `data`.
	 * If the first value of an element is negative, the corresponding
	 * element in `data` is invalid. If the most second value of an element
	 * is nonzero, the corresponding element in `data` is dirty.
	 */
	std::vector<std::array<signed int, 3>> meta;
};

#endif /* CACHE_H_INCLUDED */
