#include "Logger.h"
#include <iostream>
#include <memory>
#include <fstream>
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

void Logger::checkLogSizeAndReset() {
    std::ifstream file(currentLogFilePath);
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
        // Reiniciar el archivo de log
        std::ofstream outFile(currentLogFilePath, std::ofstream::trunc);
        if (outFile.is_open()) {
            outFile.close();
            spdlog::info("Log file reset after exceeding 5000 lines.");
        }
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