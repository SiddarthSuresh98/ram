#ifndef LOGGER_H
#define LOGGER_H
#include <fstream>
#include <sstream>
using namespace std;

enum LogLevel { DEBUG, INFO, WARNING, ERROR, CRITICAL };

class Logger
{
  public:
	static Logger* getInstance();

	~Logger() = default;

	/**
	 * Do not allow copies.
	 */
	Logger(const Logger& obj) = delete;

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
	Logger() = default;
	static Logger* logger_instance;
	static LogLevel level;
	static string level_to_string(LogLevel);
	static int level_to_int(LogLevel);
};

#endif /* LOGGER_H_INCLUDED */
