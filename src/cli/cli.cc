#include "cli.h"
#include <sstream>
#include <iostream>
#include <vector>

Cli::Cli() {
    commands["load"] = [this](std::vector<std::string> args) {
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

    commands["store"] = [this](std::vector<std::string> args) {
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

    commands["load-memory-image"] = [this](std::vector<std::string> args) {
        if (!args.empty()) {
            load_memory_image(args[0]);
        }
        else {
            std::cout << "Usage: load-memory-image <filename>\n";
        }
        return;
    };

    commands["reset"] = [this](std::vector<std::string> args) { 
        reset(); 
        return; 
    };

    commands["update-memory"] = [this](std::vector<std::string> args) {
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

    commands["view-memory"] = [this](std::vector<std::string> args) { 
        view_memory();
        return;
    };

    commands["view-memory-address"] = [this](std::vector<std::string> args) {
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

    commands["update-controls"] = [this](std::vector<std::string> args) {
        if (!args.empty()) {
            update_controls(args[0]);
        }
        else {
            std::cout << "Usage: update-controls <configuration-file>\n";
        }
        return;
    };

    commands["help"] = [this](std::vector<std::string> args) { 
        help();
        return; 
    };
}

Cli::~Cli() {}

//TODO: These function stubs are to be improved after they have been implemented internally.
void Cli::help() {
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

void Cli::load(int memory_address, int pipeline_stage) {
    std::cout << "Loading data from memory address " << memory_address
              << " at pipeline stage " << pipeline_stage << ".\n";
}


void Cli::store(int memory_address, int pipeline_stage, int data) {
    std::cout << "Storing " << data << " into memory address " << memory_address
              << " at pipeline stage " << pipeline_stage << ".\n";
}


void Cli::load_memory_image(const std::string& filename) {
    std::cout << "Loading memory image from file: " << filename << ".\n";
}


void Cli::reset() {
    std::cout << "Resetting memory configuration and cycles.\n";
}


void Cli::update_memory(int memory_address, int data) {
    std::cout << "Updating memory at address " << memory_address
              << " with data " << data << ".\n";
}


void Cli::view_memory() {
    std::cout << "Viewing current status of memory subsystem.\n";
}


void Cli::view_memory_address(int memory_address) {
    std::cout << "Viewing data at memory address " << memory_address << ".\n";
}


void Cli::update_controls(const std::string& config_file) {
    std::cout << "Updating controls using configuration file: " << config_file << ".\n";
}

void Cli::run(){
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
}

