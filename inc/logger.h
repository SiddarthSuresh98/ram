#ifndef LOGGER_H
#define LOGGER_H
#include <fstream>
#include <sstream>
using namespace std;

enum LogLevel { DEBUG, INFO, WARNING, ERROR, CRITICAL };

class Logger
{
  public:
	Logger(const string &);
	~Logger();

	void setLevel(LogLevel);
	void log(LogLevel, const string &);

  private:
	LogLevel level = INFO;
	ofstream logFile;
	string levelToString(LogLevel);
	int levelToInt(LogLevel);
};

#endif /* LOGGER_H_INCLUDED */
