#ifndef DRAM_H
#define DRAM_H
#include "definitions.h"
#include "storage.h"
#include <functional>
#include <ostream>

// clang-format off
/**
 * Ensures address is within the current memory size using a clean wrap.
 * @param an address
 */
#define WRAP_ADDRESS(a) \
  ((a < 0) ? ((a % MEM_WORDS) + MEM_WORDS) % MEM_WORDS : a % MEM_WORDS)
// clang-format on

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

	int
	write_word(void *, signed int, int) override;
	int
	write_line(void *, std::array<signed int, LINE_SIZE>, int) override;
	int
	read_word(void *, int, signed int &) override;
	int
	read_line(void *, int, std::array<signed int, LINE_SIZE> &) override;

	/**
	 * TODO This will accept a file at a later date.
	 */
	void
	load(std::vector<signed int> program);

  private:
	/**
	 * Helper for all access methods.
	 * Calls `request_handler` when `id` is allowed to complete its
	 * request cycle.
	 * @param the source making the request
	 * @param the address to write to
	 * @param the function to call when an access should be completed
	 * @return 1 if the access completed successfully, 0 otherwise
	 */
	int
	process(void *id, int address, std::function<void(int line, int word)> request_handler);
	/**
	 * Returns OK if `id` is allowed to complete its request this cycle.
	 * Handles wait times, side door, and setting the current id this
	 * storage is serving.
	 * @param the source making the request
	 * @return 1 if the access can be completed this function call, 0 otherwise
	 */
	int
	is_access_cleared(void *id);
	/**
	 * Given `address`, returns the line and word it is in.
	 * @param an address
	 * @param the line (row) `address` is in
	 * @param the word (column) `address` corresponds to
	 */
	void get_memory_index(int address, int &line, int &word);
};

#endif /* DRAM_H_INCLUDED */
