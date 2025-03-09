#include "cache.h"
#include "definitions.h"
#include "response.h"
#include <bits/stdc++.h>

Cache::Cache(int lines, Storage *lower, int delay)
{
	this->data = new std::vector<std::array<signed int, LINE_SIZE>>;
	this->data->resize(lines);
	this->lower = lower;
	this->delay = delay;
	this->lower = nullptr;
}

Cache::~Cache() { delete this->data; }

Response Cache::write(Accessor accessor, signed int data, int address)
{
	return WAIT;
}

Response Cache::read(Accessor accessor, int address, std::array<signed int, LINE_SIZE>& data) { return WAIT; }
