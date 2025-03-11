#ifndef CLI_H
#define CLI_H
#include "cache.h"
#include <functional>
#include <string>
#include <unordered_map>

class Cli
{
  public:
	/**
	 * Constructor.
	 * @return A newly allocated CLI object.
	 */
	Cli();
	~Cli();

	/**
	 * Prints all available commands to the console.
	 */
	void help();

	/**
	 * Loads data from memory from the specified memory address.
	 * @param memory_address address of the memory where data needs to be loaded
	 * from
	 */
	void load(Accessor accessor, int memory_address);

	/**
	 * Stores data into memory at the specified address.
	 * @param accessor the pipline stage that is making this request
	 * @param data data value to be written to the memory
	 * @param address address of the memory where data needs to be stored
	 * @return the response from the storage device
	 */
	void store(Accessor accessor, int data, int address);

	/**
	 * Resets the memory configuration and cycles to their initial state.
	 * This function provides a side door reset interface to the memory system,
	 * allowing the user to reset the memory configuration directly.
	 */
	void reset();

	/**
	 * Advance the clock one cycle, refreshing the storage devices.
	 */
	void clock();

	/**
	 * Displays `lines` lines of the data in `level`, starting from `base`.
	 *
	 *
	 * This function provides a side door view into the storage system, showing
	 * its current state and configuration.
	 * @param level the level specifying the storage device. The first level
	 * one cache is level zero, with descending levels incrementing by a factor
	 * of one.
	 */
	void peek(int level);

	/**
	 * Runs the command line interface
	 * This function is the main entry point for the command line interface.
	 */
	void run();

  private:
	/**
	 * Initializes the cache object.
	 */
	void initialize();
	/**
	 * Attempts to match string to either fetch or mem, or throws
	 * std::invalid_argument otherwise.
	 * @param the string to be converted accessor
	 * @return the corresponding accessor
	 * @throws invalid_argument if the string is not fetch or mem
	 */
	Accessor match_accessor_or_die(std::string s);
	/** Map of commands and their corresponding functions.
	 * This map is used to store the commands and their corresponding functions.
	 */
	std::unordered_map<char, std::function<void(std::vector<std::string>)>>
		commands;
	/**
	 * The cache object to interface with.
	 */
	Cache *cache;
	/**
	 * The current cycle.
	 */
	int cycle;
};

#endif /* CLI_H_INCLUDED */
