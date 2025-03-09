#ifndef STORAGE_H
#define STORAGE_H
#include "definitions.h"
#include "response.h"
#include <algorithm>
#include <array>
#include <vector>

enum Accessor {
	IDLE,
	MEM,
	FETCH,
	L1CACHE,
	SIDE,
};

class Storage
{
  public:
	/**
	 * Write `data` into `address`.
	 * @param the source making the request.
	 * @param the data (hexadecimal) to write.
	 * @param the address to write to.
	 * @return a status code reflecting the state of the request.
	 */
	virtual Response write(Accessor accessor, signed int data, int address) = 0;
	/**
	 * Get the data at `address`.
	 * @param the source making the request.
	 * @param the address being accessed.
	 * @return a status code reflecting the state of the request, and the
	 * data being returned.
	 */
	virtual Response read(Accessor accessor, int address) = 0;
	/**
	 * Sidedoor view of `lines` of memory starting at `base`.
	 * @param The base line to start getting memory from.
	 * @param The amount of lines to fetch.
	 * @return A matrix of data values, where each row is a line and each column
	 * is a word.
	 */
	std::vector<std::array<signed int, LINE_SIZE>> view(int base, int lines);
	/**
	 * Advances to the next job if the current job is completed.
	 */
	void resolve();

  protected:
	/**
	 * Helper for `write`.
	 */
	void do_write(signed int, int);
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
	 * The number of clock cycles this level of storage takes to complete
	 * requests.
	 */
	int delay;
	/**
	 * The accessor currently being serviced.
	 */
	enum Accessor requester;
	/**
	 * The number of cycles until the current request is completed.
	 */
	int wait_time;
};

#endif /* STORAGE_H_INCLUDED */
