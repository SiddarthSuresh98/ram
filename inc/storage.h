#ifndef STORAGE_H
#define STORAGE_H
#include "accessor.h"
#include "definitions.h"
#include "response.h"
#include <algorithm>
#include <array>
#include <map>
#include <vector>

class Storage
{
  public:
	virtual ~Storage() = default;

	/**
	 * Write `data` word into `address`.
	 * @param the source making the request.
	 * @param the data (hexadecimal) to write.
	 * @param the address to write to.
	 * @return a status code reflecting the state of the request.
	 */
	virtual Response write_word(Accessor accessor, signed int data, int address) = 0;

	/**
	 * Write a data line to given address in this level of storage
	 */
	virtual Response write_line(Accessor accessor, std::array<signed int, LINE_SIZE> data_line, int address) = 0;

	
	/**
	 * Get the data line at `address`.
	 * @param the source making the request.
	 * @param the address being accessed.
	 * @return a status code reflecting the state of the request, and the
	 * data being returned.
	 */
	virtual Response read_line(
		Accessor accessor,
		int address,
		std::array<signed int, LINE_SIZE> &data) = 0;

	/**
	 * Read a word from given address in this level of storage
	 */
	virtual Response read_word(Accessor accessor, int address, signed int &data) = 0;
	
	/**
	 * Sidedoor view of `lines` of memory starting at `base`.
	 * @param The base line to start getting memory from.
	 * @param The amount of lines to fetch.
	 * @return A matrix of data values, where each row is a line and each column
	 * is a word.
	 */
	std::vector<std::array<signed int, LINE_SIZE>>
	view(int base, int lines) const;

	/**
	 * Getter for lower attribute.
	 * TODO this doesn't seem like good object-oriented practice.
	 * @return this->lower
	 */
	Storage *get_lower();

  protected:
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
	Accessor requester;
	/**
	 * The number of cycles until the current request is completed.
	 */
	int wait_time;
	/**
	 * A flag indicating whether this level of storage is currently waiting for
	 * a lower level.
	 */
	int is_waiting;
};

#endif /* STORAGE_H_INCLUDED */
