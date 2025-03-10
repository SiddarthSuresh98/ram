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
	void load(int memory_address);

	/**
	 * Stores data into memory at the specified address.
	 * @param accessor the pipline stage that is making this request
	 * @param data data value to be written to the memory
	 * @param address address of the memory where data needs to be stored
	 */
	void store(Accessor accessor, int data, int address);

	/**
	 * Resets the memory configuration and cycles to their initial state.
	 * This function provides a side door reset interface to the memory system,
	 * allowing the user to reset the memory configuration directly.
	 */
	void reset();

	/**
	 * Updates the memory at the specified address with the given data.
	 * This function provides a side door modification interface to the memory
	 * system, allowing the user to modify the memory configuration directly.
	 * @param memory_address address of the memory to be updated
	 * @param data data value to be written to the memory
	 */
	void update(int memory_address, int data);

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
	/** Map of commands and their corresponding functions.
	 * This map is used to store the commands and their corresponding functions.
	 */
	std::unordered_map<
		std::string,
		std::function<void(std::vector<std::string>)>>
		commands;
	/**
	 * The cache object to interface with.
	 */
	Cache *cache;
};

#endif /* CLI_H_INCLUDED */
