#ifndef LOGGER_H
#define LOGGER_H

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

/**
 * @class Logger
 * @brief Singleton class to manage application logging.
 */
class Logger {
public:
    /**
     * @brief Initializes the logger with a specified log file path.
     * @param logFilePath Path to the log file.
     */
    static void init(const std::string& logFilePath);

    /**
     * @brief Retrieves the logger instance.
     * @return Shared pointer to the logger.
     */
    static std::shared_ptr<spdlog::logger> getLogger();

private:
    static std::shared_ptr<spdlog::logger> logger; ///< Shared pointer to the logger instance.
};

#endif // LOGGER_H