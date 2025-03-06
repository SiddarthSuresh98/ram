#include "logger.h"
#include <getopt.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <functional>

void err()
{
	std::cerr << "Usage:\n\trisc_vector [OPTIONS]\nOptions:\n\t--debug,\t-d: "
				 "turn on verbose output\n\t--no-python,\t-p: run without GUI\n"
			  << std::endl;
}

void parseArguments(int argc, char **argv, Logger &logger, bool &python)
{
	struct option long_options[] = {
		{"debug", no_argument, 0, 'd'},
		{"no-python", no_argument, 0, 'p'},
		{0, 0, 0, 0}};

	python = true;

	int opt;

	while ((opt = getopt_long(argc, argv, "d:p", long_options, NULL)) != -1) {
		switch (opt) {
		case 'd':
			logger.setLevel(DEBUG);
			logger.log(DEBUG, "DEBUG output enabled.");
			break;
		case 'p':
			logger.log(INFO, "Python will NOT be started!");
			python = false;
			break;
		default:
			err();
			exit(EXIT_FAILURE);
		}
	}
}

/**
 * Prints all available commands to the console.
 */
void help() {
    std::cout << "Available commands:\n"
              << "  load <memory-address> <pipeline-stage> - Load data from memory at specified address\n"
              << "  store <memory-address> <pipeline-stage> <data> - Stores data into memory at specified address\n"
              << "  load-memory-image <filename> - side door function that loads a memory image from a file and configures memory to the image\n"
              << "  reset - side door function that resets the memory configuration and cycles\n"
              << "  update-memory <memory-address> <data> - side door function that updates the memory at the specified address with data provided\n"
              << "  view-memory - side door function that views the current status of the entire memory subsystem\n"
              << "  view-memory-address <memory-address> - side door function that views data at specific memory address\n"
              << "  update-controls <configuration-file> - side door function that takes in a configuration file and updates the controls\n"
              << "  exit - Quits the program\n";
}


//TODO: These function stubs are to be improved after they have been implemented internally.

/**
 * Loads data from memory from the specified memory address.
 * @param memory_address address of the memory where data needs to be loaded from
 * @param pipeline_stage pipeline stage to be served by memory subsystem
 */
void load(int memory_address, int pipeline_stage) {
    std::cout << "Loading data from memory address " << memory_address
              << " at pipeline stage " << pipeline_stage << ".\n";
}

/**
 * Stores data into memory at the specified address.
 * @param memory_address address of the memory where data needs to be stored
 * @param pipeline_stage pipeline stage to be served by memory subsystem
 * @param data data value to be written to the memory
 */
void store(int memory_address, int pipeline_stage, int data) {
    std::cout << "Storing " << data << " into memory address " << memory_address
              << " at pipeline stage " << pipeline_stage << ".\n";
}

/**
 * Loads a memory image from a file and configures memory to the image.
 * This function provides a side door memory image loading interface to the memory system,
 * allowing the user to load a memory image from a file and configure the memory subsystem to the image.
 * @param filename name of file containing memory image
 */
void load_memory_image(const std::string& filename) {
    std::cout << "Loading memory image from file: " << filename << ".\n";
}

/**
 * Resets the memory configuration and cycles to their initial state.
 * This function provides a side door reset interface to the memory system,
 * allowing the user to reset the memory configuration directly.
 */
void reset() {
    std::cout << "Resetting memory configuration and cycles.\n";
}

/**
 * Updates the memory at the specified address with the given data.
 * This function provides a side door modification interface to the memory system,
 * allowing the user to modify the memory configuration directly.
 * @param memory_address address of the memory to be updated
 * @param data data value to be written to the memory
 */
void update_memory(int memory_address, int data) {
    std::cout << "Updating memory at address " << memory_address
              << " with data " << data << ".\n";
}

/**
 * Displays the current status of the entire memory subsystem.
 * This function provides a side door view into the memory system, 
 * showing its current state and configuration.
 */
void view_memory() {
    std::cout << "Viewing current status of memory subsystem.\n";
}

/**
 * Displays the data at the specified memory address.
 * This function provides a side door view into the memory system, 
 * showing the data at the specified memory address.
 * @param memory_address address of the memory to be viewed
 */
void view_memory_address(int memory_address) {
    std::cout << "Viewing data at memory address " << memory_address << ".\n";
}

/**
 * Updates the controls using a configuration file.
 * This function provides a side door modification interface to the control system,
 * allowing the user to update the controls directly.
 * @param config_file name of file containing control configuration
 */
void update_controls(const std::string& config_file) {
    std::cout << "Updating controls using configuration file: " << config_file << ".\n";
}


int main(int argc, char **argv)
{
	Logger logger("vector.log");
	logger.log(INFO, "Initializing...");

	bool python = true;
	parseArguments(argc, argv, logger, python);

	if (python) {
		// fork off python here
		;
		logger.log(INFO, "Python started.");
	}

    std::unordered_map<std::string, std::function<void(std::vector<std::string>)>> commands;
    
    commands["load"] = [](std::vector<std::string> args) {
        if (args.size() >= 2){
            try{
                load(std::stoi(args[0]), std::stoi(args[1]));
            } catch(const std::exception &e){
                std::cerr << "Invalid input: " << e.what() << std::endl;
            }
        }
        else {
            std::cout << "Usage: load <memory-address> <pipeline-stage>\n";
        }
        return;
    };

    commands["store"] = [](std::vector<std::string> args) {
        if (args.size() >= 3) {
            try{
                store(std::stoi(args[0]), std::stoi(args[1]), std::stoi(args[2]));
            }catch(const std::exception &e) {
                std::cerr << "Invalid input: " << e.what() << std::endl;
            }
        }
        else {
            std::cout << "Usage: store <memory-address> <pipeline-stage> <data>\n";
        }
        return;
    };

    commands["load-memory-image"] = [](std::vector<std::string> args) {
        if (!args.empty()) {
            load_memory_image(args[0]);
        }
        else {
            std::cout << "Usage: load-memory-image <filename>\n";
        }
        return;
    };

    commands["reset"] = [](std::vector<std::string> args) { 
        reset(); 
        return; 
    };

    commands["update-memory"] = [](std::vector<std::string> args) {
        if (args.size() >= 2) {
            try {
                update_memory(std::stoi(args[0]), std::stoi(args[1]));
            } catch(const std::exception &e){
                std::cerr << "Invalid input: all arguments are integers" << e.what() << std::endl;
            }
        }
        else{
            std::cout << "Usage: update-memory <memory-address> <data>\n";
        }
        return;
    };

    commands["view-memory"] = [](std::vector<std::string> args) { 
        view_memory();
        return;
    };

    commands["view-memory-address"] = [](std::vector<std::string> args) {
        if (!args.empty()) {
            try{
                view_memory_address(std::stoi(args[0]));
            }  catch(const std::exception &e){
                std::cerr << "Invalid input: " << e.what() << std::endl;
            }
        }
        else {
            std::cout << "Usage: view-memory-address <memory-address>\n";
        }
        return;
    };

    commands["update-controls"] = [](std::vector<std::string> args) {
        if (!args.empty()) {
            update_controls(args[0]);
        }
        else {
            std::cout << "Usage: update-controls <configuration-file>\n";
        }
        return;
    };

    commands["help"] = [](std::vector<std::string> args) { 
        help();
        return; 
    };

    std::cout << "Memory Command Processor Started. Type 'help' for a list of commands.\n";

    std::string input;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);
        std::istringstream iss(input);
        std::vector<std::string> tokens;
        std::string word;

        while (iss >> word) tokens.push_back(word);
        if (tokens.empty()) continue;

        std::string command = tokens[0];
        tokens.erase(tokens.begin());

        if (command == "exit") {
            std::cout << "Exiting...\n";
            break;
        }

        auto it = commands.find(command);
        if (it != commands.end()) {
            it->second(tokens);
        } else {
            std::cout << "Unknown command. Type 'help' for available commands.\n";
        }
    }

	return EXIT_SUCCESS;
}