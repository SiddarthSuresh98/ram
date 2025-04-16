#ifndef DRAM_H
#define DRAM_H
#include "definitions.h"
#include "storage.h"
#include <functional>
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

	int write_word(void *, signed int, int) override;
	int write_line(void *, std::array<signed int, LINE_SIZE>, int) override;
	int read_word(void *, int, signed int &) override;
	int read_line(void *, int, std::array<signed int, LINE_SIZE> &) override;

	/**
	 * TODO This will accept a file at a later date.
	 */
	void load(std::vector<signed int> program);

  private:
	int process(void *id, int address, std::function<void(int line, int word)> request_handler) override;
	/**
	 * Given `address`, returns the line and word it is in.
	 * @param an address
	 * @param the line (row) `address` is in
	 * @param the word (column) `address` corresponds to
	 */
	void get_memory_index(int address, int &line, int &word);
};

#endif /* DRAM_H_INCLUDED */
