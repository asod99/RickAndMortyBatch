#include "Logger.h"

std::shared_ptr<spdlog::logger> Logger::logger = nullptr;

void Logger::init(const std::string& logFilePath) {
    try {
        // Create a console sink for colored output
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink->set_level(spdlog::level::info);

        // Create a file sink for logging to file
        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFilePath, true);
        file_sink->set_level(spdlog::level::trace);

        // Combine sinks into a multi-sink logger
        logger = std::make_shared<spdlog::logger>("multi_sink", spdlog::sinks_init_list{console_sink, file_sink});
        logger->set_level(spdlog::level::trace);

        // Register the logger
        spdlog::register_logger(logger);
        spdlog::set_default_logger(logger);
        spdlog::set_pattern("[%Y-%m-%d %H:%M:%S] [%^%l%$] %v");

        spdlog::info("Logger initialized.");
    } catch (const spdlog::spdlog_ex& ex) {
        std::cerr << "Log initialization failed: " << ex.what() << std::endl;
    }
}

std::shared_ptr<spdlog::logger> Logger::getLogger() {
    return logger;
}