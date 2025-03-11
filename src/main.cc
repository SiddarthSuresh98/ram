#include "cli.h"
#include "logger.h"
#include <getopt.h>
#include <iostream>

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
  "         _/_/                                                      _/_/        \n";

static void print_version_number() { std::cout << banner << version_number << '\n'; }

static void err()
{
	std::cerr << "Usage:\n\trisc_vector [OPTIONS]\nOptions:\n\t--debug,\t-d: "
				 "turn on verbose output\n\t--no-python,\t-p: run without "
				 "GUI\n\t--version,\t-v: print the version information and exit\n"
			  << std::endl;
}

static void parseArguments(int argc, char **argv, bool &python)
{
	Logger *logger = Logger::getInstance();
	struct option long_options[] = {
		{"debug", no_argument, 0, 'd'},
		{"no-python", no_argument, 0, 'p'},
		{0, 0, 0, 0}};

	python = true;

	int opt;

	while ((opt = getopt_long(argc, argv, "d:p", long_options, NULL)) != -1) {
		switch (opt) {
		case 'd':
			logger->setLevel(DEBUG);
			logger->log(DEBUG, "DEBUG output enabled.");
			break;
		case 'p':
			logger->log(INFO, "Python will NOT be started!");
			python = false;
			break;
		case 'v':
			print_version_number();
			exit(EXIT_SUCCESS);
		default:
			err();
			exit(EXIT_FAILURE);
		}
	}
}

int main(int argc, char **argv)
{
	print_version_number();
	Logger *logger = Logger::getInstance();
	Cli cli;
	logger->log(INFO, "Initializing...");

	bool python = true;
	parseArguments(argc, argv, python);

	if (python) {
		// fork off python here
		;
		logger->log(INFO, "Python started.");
	}

	cli.run();
	return EXIT_SUCCESS;
}
