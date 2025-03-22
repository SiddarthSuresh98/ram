#ifndef DRAM_H
#define DRAM_H
#include "definitions.h"
#include "storage.h"
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

	Response
	write_word(Accessor accessor, signed int data, int address) override;
	Response read_line(
		Accessor accessor,
		int address,
		std::array<signed int, LINE_SIZE> &data_line) override;
	Response write_line(
		Accessor accessor,
		std::array<signed int, LINE_SIZE> data_line,
		int address) override;
	Response
	read_word(Accessor accessor, int address, signed int &data) override;

  private:
	/**
	 * Helper for all access methods.
	 * Calls `request_handler` when `accessor` is allowed to complete its
	 * request cycle.
	 * @param the source making the request
	 * @param the address to write to
	 * @param the function to call when an access should be completed
	 */
	Response process(
		Accessor accessor,
		int address,
		std::function<void(int line, int word)> request_handler);
	/**
	 * Returns OK if `accessor` is allowed to complete its request this cycle.
	 * Handles wait times, side door, and setting the current accessor this
	 * storage is serving.
	 * @param the accessor asking for a resource
	 * @return whether or not the access can be carried out this function call.
	 */
	Response is_access_cleared(Accessor accessor);
};

std::ostream &operator<<(std::ostream &os, const Dram &d);

#endif /* DRAM_H_INCLUDED */
