#ifndef LOGGER_H
#define LOGGER_H

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

/**
 * @class Logger
 * @brief Singleton class to manage application logging.
 *
 * This class provides static methods to initialize and manage logging
 * throughout the application using the spdlog library.
 */
class Logger {
public:
    /**
     * @brief Initializes the logger with a specified log file path and log level.
     * @param logFilePath Path to the log file.
     * @param logLevel The logging level to be set (e.g., info, debug).
     */
    static void init(const std::string& logFilePath, spdlog::level::level_enum logLevel);

    /**
     * @brief Retrieves the logger instance.
     * @return Shared pointer to the logger.
     */
    static std::shared_ptr<spdlog::logger> getLogger();

    /**
     * @brief Retrieves the current log file path.
     * @return The current log file path as a string.
     */
    static std::string getLogFilePath();

    /**
     * @brief Sets up a null logger that discards all log messages..
     */
    static void setupNullLogger();

    /**
     * @brief Checks the log file size and resets the log if necessary.
     */
    static void checkLogSizeAndReset();

private:
    static std::shared_ptr<spdlog::logger> logger; ///< Shared pointer to the logger instance.
    static std::string currentLogFilePath; ///< Path to the current log file.
};

#endif // LOGGER_H