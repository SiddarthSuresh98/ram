#include <dram.h>
#include <response.h>

Dram::Dram(int lines, int delay)
{
	this->data = new std::vector<std::array<unsigned int, 4>>;
	this->data->resize(lines);
	this->delay = delay;
	this->lower = nullptr;
}

Dram::~Dram() { delete this->data; }

Response *Dram::write(Accessor accessor, signed int data, int address)
{
	return new Response();
}

Response *Dram::read(Accessor accessor, int address) { return nullptr; }

int **Dram::view(int base, int lines) { return nullptr; }
