#ifndef CACHE_H
#define CACHE_H
#include <storage.h>

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
	Cache(int lines, Storage *lower, int delay);
	~Cache();

	Response write(Accessor accessor, signed int data, int address) override;
	Response read(Accessor accessor, int address) override;
};

#endif /* CACHE_H_INCLUDED */
