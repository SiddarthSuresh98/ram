#include <cache.h>

Cache::Cache(int lines, Storage *lower, int delay)
{
	this->data = new std::vector<std::array<unsigned int, 4>>;
	this->data->resize(lines);
	this->lower = lower;
	this->delay = delay;
	this->lower = nullptr;
}

Cache::~Cache() { delete this->data; }

Response *Cache::write(Accessor accessor, signed int data, int address)
{
	return new Response();
}

Response *Cache::read(Accessor accessor, int address) { return nullptr; }

int **Cache::view(int base, int lines) { return nullptr; }
