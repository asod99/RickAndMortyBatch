#include "Logger.h"
#include <iostream>
#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/null_sink.h>

std::shared_ptr<spdlog::logger> Logger::logger = nullptr;
std::string Logger::currentLogFilePath = "";

void Logger::init(const std::string& logFilePath, spdlog::level::level_enum logLevel) {
    try {
        // Crear un sink para logging en archivo
        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFilePath, true);
        file_sink->set_level(logLevel);

        currentLogFilePath = logFilePath;

        // Crear el logger con solo el file_sink
        logger = std::make_shared<spdlog::logger>("file_logger", file_sink);
        logger->set_level(logLevel);
        logger->flush_on(logLevel);

        // Register the logger
        spdlog::register_logger(logger);
        spdlog::set_default_logger(logger);
        spdlog::set_pattern("[%Y-%m-%d %H:%M:%S] [%^%l%$] %v");

        spdlog::info("Logger initialized.");
    } catch (const spdlog::spdlog_ex& ex) {
        std::cerr << "Log initialization failed: " << ex.what() << std::endl;
    }
}

std::string Logger::getLogFilePath() {
    return currentLogFilePath;
}

std::shared_ptr<spdlog::logger> Logger::getLogger() {
    return logger;
}

void Logger::setupNullLogger() {
    auto null_sink = std::make_shared<spdlog::sinks::null_sink_mt>();
    auto null_logger = std::make_shared<spdlog::logger>("null_logger", null_sink);
    spdlog::set_default_logger(null_logger);
}