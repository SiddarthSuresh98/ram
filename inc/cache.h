#ifndef CACHE_H
#define CACHE_H
#include "definitions.h"
#include "storage.h"
#include <array>
#include <ostream>

class Cache : public Storage
{
  public:
	/**
	 * Constructor.
	 * @param The number of `lines` contained in memory. The total number of
	 * words is this number multiplied by LINE_SIZE.
	 * @param The next lowest level in storage. Methods from this object are
	 * called in case of a cache miss.
	 * @param The number of clock cycles each access takes.
	 * @return A new cache object.
	 */
	Cache(Storage *lower, int delay);
	~Cache();

	Response
	write_word(Accessor accessor, signed int data, int address) override;
	Response write_line(
		Accessor accessor,
		std::array<signed int, LINE_SIZE> data_line,
		int address) override;
	Response read_line(
		Accessor accessor,
		int address,
		std::array<signed int, LINE_SIZE> &data_line) override;
	Response
	read_word(Accessor accessor, int address, signed int &data) override;

	/**
	 * Getter for the meta attribute.
	 * TODO this doesn't seem like good object-oriented practice.
	 * @return this->meta
	 */
	std::array<std::array<int, 2>, L1_CACHE_LINES> get_meta() const;

  private:
	/**
	 * Fetches `address` from a lower level of storage if it is not already
	 * present. If it is not, temporarily sets the is_blocked attribute of this
	 * cache level to true, and the victim line is chosen/written back.
	 * @param the address that must be present in cache.
	 */
	void handle_miss(int address);
	/**
	 * An array of metadata about elements in `data`.
	 * If the first value of an element is negative, the corresponding
	 * element in `data` is invalid. If the most second value of an element
	 * is nonzero, the corresponding element in `data` is dirty.
	 */
	std::array<std::array<int, 2>, L1_CACHE_LINES> meta;
};

std::ostream &operator<<(std::ostream &os, const Cache &c);

#endif /* CACHE_H_INCLUDED */
