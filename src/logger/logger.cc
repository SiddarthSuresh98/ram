#include "logger.h"
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

LogLevel Logger::level = INFO;
ofstream Logger::logFile;
Logger *Logger::logger_instance;

Logger::Logger(const string &filename)
{
	if (!filename.empty()) {
		logFile.open(filename, ios::app);
		if (!logFile.is_open()) {
			cerr << "Error opening log file." << endl;
		}
	}
}

Logger::~Logger() { logFile.close(); }

void Logger::setLevel(LogLevel level) { level = level; }

void Logger::log(LogLevel level, const string &message)
{
	if (level_to_int(level) > level_to_int(level)) {
		return;
	}

	time_t now = time(0);
	tm *timeinfo = localtime(&now);
	char timestamp[20];
	strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", timeinfo);

	ostringstream logEntry;
	logEntry << "[" << timestamp << "] " << level_to_string(level) << ": "
			 << message << endl;

	cout << logEntry.str();

	if (logFile.is_open()) {
		logFile << logEntry.str();
		logFile.flush();
	}
}

Logger *Logger::getInstance()
{
	if (logger_instance == nullptr) {
		logger_instance = new Logger("vector.log");
	}
	return logger_instance;
}

string Logger::level_to_string(LogLevel level)
{
	switch (level) {
	case DEBUG:
		return "DEBUG";
	case INFO:
		return "INFO";
	case WARNING:
		return "WARNING";
	case ERROR:
		return "ERROR";
	case CRITICAL:
		return "CRITICAL";
	default:
		return "UNKNOWN";
	}
}

int Logger::level_to_int(LogLevel level)
{
	switch (level) {
	case DEBUG:
		return 5;
	case INFO:
		return 4;
	case WARNING:
		return 3;
	case ERROR:
		return 2;
	case CRITICAL:
		return 1;
	default:
		return 0;
	}
}
