#include "LoggerMenu.h"
#include <spdlog/spdlog.h>
#include <iostream>
#include "Colors.h"

void showLoggerMenu() {
    int choice;
    std::string logFilePath = Logger::getLogFilePath();
    spdlog::level::level_enum logLevel = spdlog::level::info;
    bool loggerEnabled = false;

    do {
        Color::printAnimatedText("\n--- Logger Configuration Menu ---", Color::cyan);
        std::cout << Color::yellow << "1. " << Color::reset << Color::green << "Show File Path Log" << Color::reset << std::endl;
        std::cout << Color::yellow << "2. " << Color::reset << Color::green << "Set log level" << Color::reset << std::endl;
        std::cout << Color::yellow << "3. " << Color::reset << Color::green << "Set log file path" << Color::reset << std::endl;
        std::cout << Color::yellow << "4. " << Color::reset << Color::green << "Enable/Disable logger" << Color::reset << std::endl;
        std::cout << Color::yellow << "5. " << Color::reset << Color::green << "Return to configuration menu" << Color::reset << std::endl;
        std::cout << Color::magenta << "Choose an option: " << Color::reset;
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cout << Color::red << "Invalid input. Please enter a number." << Color::reset << std::endl;
            std::cin.clear();  // Limpiamos el estado de error
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Limpiamos el buffer de entrada
            continue;
        }

        switch (choice) {
            case 1:
                std::cout << Color::cyan << "Log file location: " << logFilePath << Color::reset << std::endl;
                break;
            case 2: {
                int levelChoice;
                Color::printAnimatedText("Select log level:", Color::cyan);
                std::cout << Color::yellow << "1. " << Color::reset << "Trace" << std::endl;
                std::cout << Color::yellow << "2. " << Color::reset << "Debug" << std::endl;
                std::cout << Color::yellow << "3. " << Color::reset << "Info" << std::endl;
                std::cout << Color::yellow << "4. " << Color::reset << "Warn" << std::endl;
                std::cout << Color::yellow << "5. " << Color::reset << "Error" << std::endl;
                std::cout << Color::yellow << "6. " << Color::reset << "Critical" << std::endl;
                std::cout << Color::yellow << "7. " << Color::reset << "Off" << std::endl;
                std::cout << Color::magenta << "Choose an option: " << Color::reset;
                std::cin >> levelChoice;

                if (std::cin.fail()) {
                    std::cout << Color::red << "Invalid input. Please enter a number." << Color::reset << std::endl;
                    std::cin.clear();  // Limpiamos el estado de error
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Limpiamos el buffer de entrada
                    continue;
                }

                switch (levelChoice) {
                    case 1: logLevel = spdlog::level::trace; break;
                    case 2: logLevel = spdlog::level::debug; break;
                    case 3: logLevel = spdlog::level::info; break;
                    case 4: logLevel = spdlog::level::warn; break;
                    case 5: logLevel = spdlog::level::err; break;
                    case 6: logLevel = spdlog::level::critical; break;
                    case 7: logLevel = spdlog::level::off; break;
                    default: 
                        std::cout << Color::red << "Invalid option. Please try again." << Color::reset << std::endl; 
                        continue;
                }
                std::cout << Color::green << "Log level set." << Color::reset << std::endl;
                break;
            }
            case 3: {
                std::cout << Color::cyan << "Enter log file path: " << Color::reset;
                std::cin >> logFilePath;
                std::cout << Color::green << "Log file path set to " << logFilePath << "." << Color::reset << std::endl;
                break;
            }
            case 4: {
                loggerEnabled = !loggerEnabled;
                if (logFilePath.empty()) {
                    logFilePath = (std::filesystem::current_path() / "default.log").string();
                    std::cout << Color::yellow << "No log file path specified. Using default: " << logFilePath << Color::reset << std::endl;
                }
                enableLogger(loggerEnabled, logFilePath, logLevel);
                std::cout << Color::green << "Logger " << (loggerEnabled ? "enabled" : "disabled") << "." << Color::reset << std::endl;
                break;
            }
            case 5:
                std::cout << Color::cyan << "Returning to configuration menu..." << Color::reset << std::endl;
                break;
            default:
                std::cout << Color::red << "Invalid option. Please try again." << Color::reset << std::endl;
                break;
        }
    } while (choice != 5);
}

void enableLogger(bool loggerEnabled, const std::string& logFilePath, spdlog::level::level_enum logLevel) {
    if (loggerEnabled) {
        Logger::init(logFilePath, logLevel);
    } else {
        Logger::setupNullLogger();
    }
}