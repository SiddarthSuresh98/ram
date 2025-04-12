#ifndef CACHE_H
#define CACHE_H
#include "definitions.h"
#include "storage.h"
#include <array>
#include <functional>
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

	int
	write_word(void *, signed int, int) override;
	int
	write_line(void *, std::array<signed int, LINE_SIZE>, int) override;
	int
	read_line(void *, int, std::array<signed int, LINE_SIZE> &) override;
	int
	read_word(void *, int, signed int &) override;

	/**
	 * Getter for the meta attribute.
	 * TODO this doesn't seem like good object-oriented practice.
	 * @return this->meta
	 */
	std::array<std::array<int, 2>, L1_CACHE_LINES>
	get_meta() const;

  private:
	/**
	 * Helper for all access methods.
	 * Calls `request_handler` when `id` is allowed to complete its
	 * request cycle.
	 * @param the source making the request
	 * @param the address to write to
	 * @param the function to call when an access should be completed
	 */
	int
	process(void *id, int address, std::function<void(int index, int offset)> request_handler);
	/**
	 * Returns OK if `id` is allowed to complete its request this cycle.
	 * Handles cache misses, wait times, and setting the current id this
	 * storage is serving.
	 * @param the id asking for a resource
	 * @return 1 if the access can be carried out this function call, 0 otherwise.
	 */
	int
	is_access_cleared(void *id, int address);
	/**
	 * Helper for is_access_cleared.
	 * Fetches `address` from a lower level of storage if it is not already
	 * present. The victim line is chosen/written back.
	 * @param the address that must be present in cache.
	 * @param 0 if the address is currently in cache, 1 if it is being fetched.
	 */
	int
	is_address_missing(int address);
	/**
	 * An array of metadata about elements in `data`.
	 * If the first value of an element is negative, the corresponding
	 * element in `data` is invalid. If the most second value of an element
	 * is nonzero, the corresponding element in `data` is dirty.
	 */
	std::array<std::array<int, 2>, L1_CACHE_LINES> meta;
};

#endif /* CACHE_H_INCLUDED */
