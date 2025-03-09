#include "Logger.h"
#include <iostream>
#include <memory>
#include <fstream>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/null_sink.h>

std::shared_ptr<spdlog::logger> Logger::_logger = nullptr;
std::string Logger::_currentLogFilePath = "";

void Logger::init(const std::string& logFilePath, spdlog::level::level_enum logLevel) {
    try {
        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFilePath, true);
        file_sink->set_level(logLevel);

        _currentLogFilePath = logFilePath;

        _logger = std::make_shared<spdlog::logger>("file_logger", file_sink);
        _logger->set_level(logLevel);
        _logger->flush_on(logLevel);

        spdlog::register_logger(_logger);
        spdlog::set_default_logger(_logger);
        spdlog::set_pattern("[%Y-%m-%d %H:%M:%S] [%^%l%$] %v");

        spdlog::info("Logger initialized with file path: {}", logFilePath);
    } catch (const spdlog::spdlog_ex& ex) {
        std::cerr << "Log initialization failed: " << ex.what() << std::endl;
    }
}

void Logger::checkLogSizeAndReset() {
    std::ifstream file(_currentLogFilePath);
    if (!file.is_open()) {
        return;
    }

    std::string line;
    size_t line_count = 0;
    while (std::getline(file, line)) {
        ++line_count;
    }

    file.close();

    if (line_count > 5000) {
        std::ofstream outFile(_currentLogFilePath, std::ofstream::trunc);
        if (outFile.is_open()) {
            outFile.close();
            spdlog::info("Log file reset after exceeding 5000 lines.");
        }
    }
}

std::string Logger::getLogFilePath() {
    return _currentLogFilePath;
}

void Logger::setupNullLogger() {
    auto null_sink = std::make_shared<spdlog::sinks::null_sink_mt>();
    auto null_logger = std::make_shared<spdlog::logger>("null_logger", null_sink);
    spdlog::set_default_logger(null_logger);
}