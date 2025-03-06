#ifndef STORAGE_H
#define STORAGE_H
#include "response.h"
#include <array>
#include <vector>

enum Accessor {
	MEMORY,
	FETCH,
	L1CACHE,
};

class Storage
{
  public:
	/**
	 * Write `data` into `address`.
	 * @param the source making the request.
	 * @param the data (hexadecimal) to write.
	 * @param the address to write to.
	 * @return a status code reflecting the state of the storage level.
	 */
	virtual Response *
	write(Accessor accessor, signed int data, int address) = 0;
	/**
	 * Get the data at `address`.
	 * @param the source making the request.
	 * @param the address being accessed.
	 * @return a status code reflecting the state of the storage level, and the
	 * data being returned.
	 */
	virtual Response *read(Accessor accessor, int address) = 0;
	/**
	 * Sidedoor view of `lines` of memory starting at `base`.
	 * @param The base line to start getting memory from.
	 * @param The amount of lines to fetch.
	 * @return A matrix of data values, where each row is a line and each column
	 * is a word.
	 */
	virtual int **view(int base, int lines) = 0;

  protected:
	/**
	 * The data currently stored in this level of storage.
	 */
	std::vector<std::array<unsigned int, 4>> *data;
	/**
	 * A pointer to the next lowest level of storage.
	 * Used in case of cache misses.
	 */
	Storage *lower;
	/**
	 * The number of clock cycles this level of storage takes to complete
	 * requests.
	 */
	int delay;
};

#endif /* STORAGE_H_INCLUDED */
