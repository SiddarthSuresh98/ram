#ifndef CLI_H
#define CLI_H
#include <unordered_map>
#include <functional>
#include <string>

class Cli {
    public:
        
        Cli();
        
        ~Cli();
        
        /**
        * Prints all available commands to the console.
        */
        void help();
        
        /**
        * Loads data from memory from the specified memory address.
        * @param memory_address address of the memory where data needs to be loaded from
        * @param pipeline_stage pipeline stage to be served by memory subsystem
        */
        void load(int memory_address, int pipeline_stage);
    
        /**
        * Stores data into memory at the specified address.
        * @param memory_address address of the memory where data needs to be stored
        * @param pipeline_stage pipeline stage to be served by memory subsystem
        * @param data data value to be written to the memory
        */
        void store(int memory_address, int pipeline_stage, int data);
    
        /**
        * Loads a memory image from a file and configures memory to the image.
        * This function provides a side door memory image loading interface to the memory system,
        * allowing the user to load a memory image from a file and configure the memory subsystem to the image.
        * @param filename name of file containing memory image
        */
        void load_memory_image(const std::string& filename);
    
        /**
        * Resets the memory configuration and cycles to their initial state.
        * This function provides a side door reset interface to the memory system,
        * allowing the user to reset the memory configuration directly.
        */
        void reset();
    
        /**
        * Updates the memory at the specified address with the given data.
        * This function provides a side door modification interface to the memory system,
        * allowing the user to modify the memory configuration directly.
        * @param memory_address address of the memory to be updated
        * @param data data value to be written to the memory
        */
        void update_memory(int memory_address, int data);
    
        /**
        * Displays the current status of the entire memory subsystem.
        * This function provides a side door view into the memory system, 
        * showing its current state and configuration.
        */
        void view_memory();
    
        /**
        * Displays the data at the specified memory address.
        * This function provides a side door view into the memory system, 
        * showing the data at the specified memory address.
        * @param memory_address address of the memory to be viewed
        */
        void view_memory_address(int memory_address);
    
        /**
        * Updates the controls using a configuration file.
        * This function provides a side door modification interface to the control system,
        * allowing the user to update the controls directly.
        * @param config_file name of file containing control configuration
        */
        void update_controls(const std::string& config_file);

        /** 
         * Runs the command line interface
         * This function is the main entry point for the command line interface.
         */
        void run();

    private:

        /** Map of commands and their corresponding functions.
         * This map is used to store the commands and their corresponding functions.
        */
        std::unordered_map<std::string, std::function<void(std::vector<std::string>)>> commands;
};

#endif /* CLI_H_INCLUDED */