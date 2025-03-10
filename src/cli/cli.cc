#include "cli.h"
#include "cache.h"
#include "definitions.h"
#include "dram.h"
#include <iostream>
#include <sstream>
#include <vector>

Cli::Cli()
{
	Dram *d = new Dram(MEM_SIZE, MEM_DELAY);
	this->cache = new Cache(d, L1_CACHE_DELAY);

	commands["l"] = [this](std::vector<std::string> args) {
		if (args.size() >= 1) {
			try {
				load(std::stoi(args[0]));
			} catch (const std::exception &e) {
				std::cerr << "Invalid input: " << e.what() << std::endl;
			}
		} else {
			std::cout << "Usage: l <memory-address>\n";
		}
		return;
	};

	commands["s"] = [this](std::vector<std::string> args) {
		if (args.size() >= 2) {
			try {
				store(std::stoi(args[0]), std::stoi(args[1]));
			} catch (const std::exception &e) {
				std::cerr << "Invalid input: " << e.what() << std::endl;
			}
		} else {
			std::cout << "Usage: s <memory-address> <data>\n";
		}
		return;
	};

	commands["r"] = [this](std::vector<std::string> args) {
		reset();
		return;
	};

	commands["v"] = [this](std::vector<std::string> args) {
		if (args.size() >= 3) {
			try {
				view(
					std::stoi(args[0]), std::stoi(args[1]), std::stoi(args[2]));
			} catch (const std::exception &e) {
				std::cerr << "Invalid input: " << e.what() << std::endl;
			}
		} else {
			std::cout << "Usage: v <storage-level> <base> <lines>\n";
		}
		return;
	};

	commands["h"] = [this](std::vector<std::string> args) {
		help();
		return;
	};
}

Cli::~Cli() { delete this->cache; }

void Cli::help()
{
	std::cout
		<< "Available commands:\n"
		<< "  l <memory-address> - Load data from memory at the specified "
		   "address\n"
		<< "  s <memory-address> <data> - Stores data into "
		   "memory at specified address\n"
		<< "  c - manually advances the clock\n"
		<< "  r - side door function that resets the memory configuration and "
		   "cycles\n"
		<< "  u <memory-address> <data> - side door function that updates "
		   "the memory at the specified address with data provided\n"
		<< "  v <storage-level> <base> <lines> - side door function that views "
		   "the current status of the entire memory subsystem\n"
		<< "  h - Prints this help text\n"
		<< "  q - Quits the program\n";
}

void Cli::load(int memory_address)
{
	std::cout << "Loading data from memory address " << memory_address;
}

void Cli::store(int memory_address, int data)
{
	std::cout << "Storing " << data << " into memory address "
			  << memory_address;
}

void Cli::update(int memory_address, int data)
{
	std::cout << "Resetting memory configuration and cycles.\n";
};

void Cli::clock()
{
	std::cout << "Resetting memory configuration and cycles.\n";
}

void Cli::reset()
{
	std::cout << "Resetting memory configuration and cycles.\n";
}

void Cli::view(int level, int base, int lines)
{
	Storage *curr = this->cache;
	for (int i = 0; i < level; ++i) {
		if (!curr) {
			std::cerr << "Level " << level << " of storage does not exist.\n";
			return;
		}
		curr = curr->get_lower();
	}

	Cache *c = dynamic_cast<Cache *>(curr);
	if (c) {
		std::cout << *c;
	} else {
		std::cout << "dram";
	}
}

void Cli::run()
{
	std::cout << "Memory Command Processor Started. Type 'h' for a list of "
				 "commands.\n";

	std::string input;
	while (true) {
		std::cout << "> ";
		std::getline(std::cin, input);
		std::istringstream iss(input);
		std::vector<std::string> tokens;
		std::string word;

		while (iss >> word)
			tokens.push_back(word);
		if (tokens.empty())
			continue;

		std::string command = tokens[0];
		tokens.erase(tokens.begin());

		if (command == "q") {
			std::cout << "Exiting...\n";
			break;
		}

		auto it = commands.find(command);
		if (it != commands.end()) {
			it->second(tokens);
		} else {
			std::cout
				<< "Unknown command. Type 'help' for available commands.\n";
		}
	}
}

void Cli::initialize()
{
	if (this->cache)
		delete this->cache;
}
