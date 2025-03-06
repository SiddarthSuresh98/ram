#ifndef DRAM_H
#define DRAM_H
#include <storage.h>

class Dram : public Storage
{
  public:
	/**
	 * Constructor.
	 * @param The number of `lines` contained in memory. The total number of
	 * words is this number multiplied by 4.
	 * @param The number of clock cycles each access takes.
	 * @return A new memory object.
	 */
	Dram(int lines, int delay);
	~Dram();

	Response *write(Accessor accessor, signed int data, int address) override;
	Response *read(Accessor accessor, int address) override;
	int **view(int base, int lines) override;
};

#endif /* DRAM_H_INCLUDED */
