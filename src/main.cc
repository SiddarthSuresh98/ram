#include "cli.h"
#include "definitions.h"
#include "logger.h"
#include <getopt.h>
#include <iostream>

static Logger *global_log = Logger::getInstance();

static std::string version_number = "v0.1";
static std::string banner =
  "    _/_/_/    _/_/_/    _/_/_/    _/_/_/   \n"
  "   _/    _/    _/    _/        _/          \n"
  "  _/_/_/      _/      _/_/    _/           \n"
  " _/    _/    _/          _/  _/            \n"
  "_/    _/  _/_/_/  _/_/_/      _/_/_/       \n"
  "                                           \n"
  "                                           \n"
  "               _/_/                                                      _/_/  \n"
  "  _/      _/  _/    _/_/_/_/    _/_/_/  _/_/_/_/_/    _/_/    _/_/_/      _/   \n"
  " _/      _/  _/    _/        _/            _/      _/    _/  _/    _/    _/    \n"
  "_/      _/  _/    _/_/_/    _/            _/      _/    _/  _/_/_/      _/     \n"
  " _/  _/    _/    _/        _/            _/      _/    _/  _/    _/    _/      \n"
  "  _/      _/    _/_/_/_/    _/_/_/      _/        _/_/    _/    _/    _/       \n"
  "         _/_/                                                      _/_/    ";
static void print_version_number()
{
	std::cout << banner << version_number << '\n';
}

static void err()
{
	std::cerr << "Usage:\n\trisc_vector [OPTIONS]\nOptions:\n\t--debug,\t-d: "
				 "turn on verbose output\n\t--memory-only,\t-m: run the memory "
				 "simulator only, without a GUI.\n\t--version,\t-v: print the "
				 "version information and exit\n"
			  << std::endl;
}

static void
parseArguments(int argc, char **argv, bool &memory_only)
{
	struct option long_options[] = {
		{"debug", no_argument, 0, 'd'},
		{"memory-only", no_argument, 0, 'm'},
		{0, 0, 0, 0}};

	int opt;

	while ((opt = getopt_long(argc, argv, "d:m", long_options, NULL)) != -1) {
		switch (opt) {
		case 'd':
			global_log->setLevel(DEBUG);
			global_log->log(DEBUG, "DEBUG output enabled.");
			break;
		case 'm':
			global_log->log(INFO, "Starting the storage CLI interface...");
			memory_only = true;
			break;
		default:
			err();
			exit(EXIT_FAILURE);
		}
	}
}

int main(int argc, char **argv)
{
	print_version_number();
	global_log->log(INFO, "Initializing...");

	bool memory_only = false;
	parseArguments(argc, argv, memory_only);

	if (memory_only) {
		Cli cli;
		cli.run();
	}

	// fork off python here
	global_log->log(INFO, "Python started.");
	global_log->log(INFO, "Cleaning up...");
	global_log->log(INFO, "Goodbye!");
	return EXIT_SUCCESS;
}
