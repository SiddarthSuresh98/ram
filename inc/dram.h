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
	 * @param The number of `lines` contained in memory. The total number of
	 * words is this number multiplied by LINE_SIZE.
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
	 * Helper for `write` a word
	 */
	void do_write(signed int, int);
	/**
	 * Helper for writing a line.
	 */
	void
	do_write_line(std::array<signed int, LINE_SIZE> data_line, int address);
	/**
	 * Helper for `read` a line
	 */
	void do_read(std::array<signed int, LINE_SIZE> &data_line, int address);
	/**
	 * Helper for reading a word.
	 */
	void do_read_word(signed int &data, int address);
};

std::ostream &operator<<(std::ostream &os, const Dram &d);

#endif /* DRAM_H_INCLUDED */
