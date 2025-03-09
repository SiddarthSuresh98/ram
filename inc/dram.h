#ifndef DRAM_H
#define DRAM_H
#include <storage.h>

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
	Dram(int lines, int delay);
	~Dram();

	Response write(Accessor accessor, signed int data, int address) override;
	Response read(Accessor accessor, int address, std::array<signed int, LINE_SIZE>& data) override;

	private:
	void do_read(std::array<signed int, LINE_SIZE>& data_line, int address);
};

#endif /* DRAM_H_INCLUDED */
