#ifndef LOGGERMENU_H
#define LOGGERMENU_H

#include "Logger.h"
#include <iostream>
#include <filesystem>

void showLoggerMenu();
void enableLogger(bool loggerEnabled,const std::string& logFilePath, spdlog::level::level_enum logLevel);

#endif // LOGGERMENU_H