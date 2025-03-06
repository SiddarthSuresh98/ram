#ifndef LOGGER_H
#define LOGGER_H
#include <fstream>
#include <sstream>
using namespace std;

enum LogLevel { DEBUG, INFO, WARNING, ERROR, CRITICAL };

class Logger
{
  public:
	/**
	 * Constructor.
	 * @param The file name to log to.
	 * @return A new logger object.
	 */
	Logger(const string &);
	~Logger();

	/**
	 * Set the log level.
	 * @param the log level to set to.
	 */
	void setLevel(LogLevel);
	/**
	 * Log a message at a certain log level.
	 * @param The level to log this message. If the level is lower than the
	 * level set by `setLevel`, then the message is not logged.
	 * @param The message to log.
	 */
	void log(LogLevel, const string &);

  private:
	LogLevel level = INFO;
	ofstream logFile;
	string levelToString(LogLevel);
	int levelToInt(LogLevel);
};

#endif /* LOGGER_H_INCLUDED */
