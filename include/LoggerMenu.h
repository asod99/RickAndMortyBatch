#ifndef LOGGERMENU_H
#define LOGGERMENU_H

#include "Logger.h"
#include <iostream>
#include <filesystem>

/**
 * @brief Displays the logger menu.
 */
void showLoggerMenu();

/**
 * @brief Enables or disables the logger based on user input.
 * @param loggerEnabled Boolean indicating whether the logger should be enabled.
 * @param logFilePath Path to the log file if logging is enabled.
 * @param logLevel The logging level to set if logging is enabled.
 */
void enableLogger(bool loggerEnabled, const std::string& logFilePath, spdlog::level::level_enum logLevel);

#endif // LOGGERMENU_H