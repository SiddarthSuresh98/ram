#include "cli.h"
#include "logger.h"
#include <getopt.h>
#include <iostream>

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

int main(int argc, char **argv)
{
	Logger logger("vector.log");
	Cli cli;
	logger.log(INFO, "Initializing...");

	bool python = true;
	parseArguments(argc, argv, logger, python);

	if (python) {
		// fork off python here
		;
		logger.log(INFO, "Python started.");
	}

	cli.run();
	return EXIT_SUCCESS;
}
