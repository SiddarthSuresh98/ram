#include "cli.h"
#include "cache.h"
#include "definitions.h"
#include "dram.h"
#include "response.h"
#include "utils.h"
#include <iostream>

Cli::Cli()
{
	this->cache = nullptr;
	this->cycle = 0;
	this->initialize();

	commands['l'] = [this](std::vector<std::string> args) {
		Accessor a;
		if (args.size() >= 2) {
			try {
				a = match_accessor_or_die(args[0]);
				load(a, std::stoi(args[1]));
			} catch (const std::invalid_argument &e) {
				std::cerr << "Invalid input: " << e.what() << std::endl;
			}
		} else {
			std::cout << "Usage: l <memory-address>\n";
		}
		return;
	};

	commands['s'] = [this](std::vector<std::string> args) {
		Accessor a;
		if (args.size() >= 3) {
			try {
				a = match_accessor_or_die(args[0]);
				store(a, std::stoi(args[1]), std::stoi(args[2]));
			} catch (const std::invalid_argument &e) {
				std::cerr << "Invalid input: " << e.what() << std::endl;
			}
		} else {
			std::cout << "Usage: s <memory-address> <data>\n";
		}
		return;
	};

	commands['r'] = [this](std::vector<std::string> args) {
		reset();
		return;
	};

	commands['p'] = [this](std::vector<std::string> args) {
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

	commands['c'] = [this](std::vector<std::string> args) {
		clock();
		return;
	};

	commands['h'] = [this](std::vector<std::string> args) {
		help();
		return;
	};
}

Cli::~Cli() { delete this->cache; }

void Cli::help()
{
	std::cout
		<< "Available commands:" << std::endl
		<< "  [l]oad <address> - Load data from memory at the specified "
		   "address"
		<< std::endl
		<< "  [s]tore <accessor> <data> <address> - Stores data into memory at "
		   "specified address. Acessor must be one of: [f]etch, [m]em"
		<< "  [p]eek <storage-level> <base> <lines> - side door function that "
		   "peeks the current status of the entire memory subsystem"
		<< std::endl
		<< std::endl
		<< "  [c]ycle - manually advances the clock" << std::endl
		<< "  [f]orce - advances the clock until one operation reports "
		   "completion"
		<< std::endl
		<< "  [r]eset - side door function that resets the memory "
		   "configuration and "
		   "cycles"
		<< std::endl
		<< "  [h]elp  - prints this help text" << std::endl
		<< "  [q]uit  - quits the program" << std::endl;
}

void Cli::load(Accessor accessor, int address)
{
	const auto default_flags = std::cout.flags();
	const auto default_fill = std::cout.fill();

	signed int data;
	// Response r = this->cache->read_word(accessor, address, data);
	// std::cout << r << " to " << accessor << " reading " << address << std::endl;
	// if (r == OK)
	// 	std::cout << "\tGot:" << std::hex << data;

	std::cout.flags(default_flags);
	std::cout.fill(default_fill);
}

void Cli::store(Accessor accessor, int data, int address)
{
	Response r = this->cache->write_word(accessor, data, address);
	std::cout << r << " to " << accessor << " storing " << data << " in"
			  << address << std::endl;
}

void Cli::clock()
{
	this->cache->resolve();
	++this->cycle;
}

void Cli::reset()
{
	this->initialize();
	std::cout << "Done." << std::endl;
}

void Cli::peek(int level)
{
	Storage *curr = this->cache;
	for (int i = 0; i < level; ++i) {
		if (!curr) {
			std::cerr << "Level " << level << " of storage does not exist."
					  << std::endl;
			return;
		}
		curr = curr->get_lower();
	}

	Cache *c = dynamic_cast<Cache *>(curr);
	if (c) {
		std::cout << *c << std::endl;
	} else {
		std::cout << *dynamic_cast<Dram *>(curr) << std::endl;
		;
	}
}

void Cli::run()
{
	std::cout << "Memory Command Processor Started. Type 'h' for a list of "
				 "commands."
			  << std::endl;
	std::string input;

	bool run = true;
	while (run) {
		std::cout << this->cycle << "> ";
		std::getline(std::cin, input);

		std::istringstream iss1(input);
		std::vector<std::string> words;
		std::string sentence;
		std::string word;

		while (std::getline(iss1, sentence, ';')) {
			words.clear();
			std::istringstream iss2(sentence);

			while (iss2 >> word) {
				words.push_back(word);
			}
			if (words.empty())
				continue;

			std::string command = words[0];
			words.erase(words.begin());

			if (command == "q") {
				run = false;
				break;
			}

			auto it = commands.find(tolower(command[0]));
			if (it != commands.end()) {
				it->second(words);
			} else {
				std::cout << "Unknown command: '" << command
						  << "'. Type 'help' for available commands."
						  << std::endl;
			}
		}
	}
}

void Cli::initialize()
{
	Logger *global_log = Logger::getInstance();

	global_log->log(INFO, "Resetting memory configuration and cycle.");

	if (this->cache != nullptr)
		delete this->cache;

	Dram *d = new Dram(MEM_SIZE, MEM_DELAY);
	this->cache = new Cache(d, L1_CACHE_DELAY);
	this->cycle = 1;
}

Accessor Cli::match_accessor_or_die(std::string s)
{
	if (tolower(s[0]) == 'f')
		return FETCH;
	else if (tolower(s[0]) == 'm')
		return MEM;
	else
		throw std::invalid_argument(s);
}
