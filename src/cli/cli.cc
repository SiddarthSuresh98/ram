#include "cli.h"
#include "cache.h"
#include "definitions.h"
#include "dram.h"
#include "response.h"
#include <stdio.h>

Cli::Cli()
{
	this->cache = nullptr;
	this->initialize();

	commands["l"] = [this](std::vector<std::string> args) {
		if (args.size() >= 1) {
			try {
				load(std::stoi(args[0]));
			} catch (const std::invalid_argument &e) {
				std::cerr << "Invalid input: " << e.what() << std::endl;
			}
		} else {
			std::cout << "Usage: l <memory-address>\n";
		}
		return;
	};

	commands["s"] = [this](std::vector<std::string> args) {
		Accessor a;
		if (args.size() >= 3) {
			try {
				if (tolower(args[0][0]) == 'f')
					a = FETCH;
				else if (tolower(args[0][0]) == 'm')
					a = MEM;
				else
					throw std::invalid_argument(args[0]);
				store(a, std::stoi(args[1]), std::stoi(args[2]));
			} catch (const std::invalid_argument &e) {
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

	commands["p"] = [this](std::vector<std::string> args) {
		if (args.size() >= 1) {
			try {
				peek(std::stoi(args[0]));
			} catch (const std::invalid_argument &e) {
				std::cerr << "Invalid input: " << e.what() << std::endl;
			}
		} else {
			std::cout << "Usage: v <storage-level> <base> <lines>\n";
		}
		return;
	};

	commands["c"] = [this](std::vector<std::string> args) {
		clock();
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
		<< "  l <address> - Load data from memory at the specified "
		   "address\n"
		<< "  s <accessor> <data> <address> - Stores data into memory at "
		   "specified address. Accessor must be one of: \"MEM\", \"FETCH\", "
		   "\"L1CACHE\".\n"
		<< "  c - manually advances the clock\n"
		<< "  f - advances the clock until one operation reports completion\n"
		<< "  r - side door function that resets the memory configuration and "
		   "cycles\n"
		<< "  p <storage-level> <base> <lines> - side door function that peeks "
		   "the current status of the entire memory subsystem\n"
		<< "  h - Prints this help text\n"
		<< "  q - Quits the program\n";
}

void Cli::load(int memory_address) { ; }

void Cli::store(Accessor accessor, int data, int address)
{
	Response r = this->cache->write(accessor, data, address);
	std::cout << r << " to " << accessor << " storing " << data << '\n';
}

void Cli::update(int memory_address, int data)
{
	std::cout << "Resetting memory configuration and cycles.\n";
};

void Cli::clock() { this->cache->resolve(); }

void Cli::reset()
{
	this->initialize();
	std::cout << "Done.\n";
}

void Cli::peek(int level)
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
		std::cout << *dynamic_cast<Dram *>(curr);
		;
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
			std::cout << "Unknown command: '" << command
					  << "'. Type 'help' for available commands.\n";
		}
	}
}

void Cli::initialize()
{
	Logger *global_log = Logger::getInstance();
	global_log->log(INFO, "Resetting memory configuration.");
	if (this->cache != nullptr)
		delete this->cache;
	Dram *d = new Dram(MEM_SIZE, MEM_DELAY);
	this->cache = new Cache(d, L1_CACHE_DELAY);
}
