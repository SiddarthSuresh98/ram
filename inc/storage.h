#ifndef STORAGE_H
#define STORAGE_H
#include "definitions.h"
#include <algorithm>
#include <array>
#include <map>
#include <vector>

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
	virtual int
	write_word(void *id, signed int data, int address) = 0;
	virtual int
	write_line(void *id, std::array<signed int, LINE_SIZE> data_line, int address) = 0;

	/**
	 * Get the data line at `address`.
	 * @param the source making the request.
	 * @param the address being accessed.
	 * @param the data being returned
	 * @return 1 if the request was completed, 0 otherwise
	 */
	virtual int
	read_line(void *id, int address, std::array<signed int, LINE_SIZE> &data) = 0;
	virtual int
	read_word(void *id, int address, signed int &data) = 0;

	/**
	 * Sidedoor view of `lines` of memory starting at `base`.
	 * @param The base line to start getting memory from.
	 * @param The amount of lines to fetch.
	 * @return A matrix of data values, where each row is a line and each column
	 * is a word.
	 */
	std::vector<std::array<signed int, LINE_SIZE>>
	view(int base, int lines) const;

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
