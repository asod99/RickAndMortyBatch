#include "LoggerMenu.h"
#include <spdlog/spdlog.h>

void showLoggerMenu() {
    int choice;
    std::string logFilePath = Logger::getLogFilePath();
    spdlog::level::level_enum logLevel = spdlog::level::info;
    bool loggerEnabled = false;

    do {
        std::cout << "\n--- Logger Configuration Menu ---\n";
        std::cout << "1. Show File Path Log\n";
        std::cout << "2. Set log level\n";
        std::cout << "3. Set log file path\n";
        std::cout << "4. Enable/Disable logger\n";
        std::cout << "5. Return to configuration menu\n";
        std::cout << "Choose an option: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                std::cout << "Log file location: " << logFilePath << "\n";
                break;
            case 2: {
                int levelChoice;
                std::cout << "Select log level:\n";
                std::cout << "1. Trace\n";
                std::cout << "2. Debug\n";
                std::cout << "3. Info\n";
                std::cout << "4. Warn\n";
                std::cout << "5. Error\n";
                std::cout << "6. Critical\n";
                std::cout << "7. Off\n";
                std::cout << "Choose an option: ";
                std::cin >> levelChoice;

                switch (levelChoice) {
                    case 1: logLevel = spdlog::level::trace; break;
                    case 2: logLevel = spdlog::level::debug; break;
                    case 3: logLevel = spdlog::level::info; break;
                    case 4: logLevel = spdlog::level::warn; break;
                    case 5: logLevel = spdlog::level::err; break;
                    case 6: logLevel = spdlog::level::critical; break;
                    case 7: logLevel = spdlog::level::off; break;
                    default: std::cout << "Invalid option. Please try again.\n"; break;
                }
                std::cout << "Log level set.\n";
                break;
            }
            case 3: {
                std::cout << "Enter log file path: ";
                std::cin >> logFilePath;
                std::cout << "Log file path set to " << logFilePath << ".\n";
                break;
            }
            case 4: {
                loggerEnabled = !loggerEnabled;
                if (logFilePath.empty()) {
                    logFilePath = (std::filesystem::current_path() / "default.log").string();
                    std::cout << "No log file path specified. Using default: " << logFilePath << "\n";
                }
                enableLogger(loggerEnabled,logFilePath, logLevel);

                std::cout << "Logger " << (loggerEnabled ? "enabled" : "disabled") << ".\n";
                break;
            }
            case 5:
                std::cout << "Returning to configuration menu...\n";
                break;
            default:
                std::cout << "Invalid option. Please try again.\n";
                break;
        }
    } while (choice != 5);

}

void enableLogger(bool loggerEnabled,const std::string& logFilePath, spdlog::level::level_enum logLevel)
{
    if (loggerEnabled) {
        Logger::init(logFilePath, logLevel);
    } else {
        Logger::setupNullLogger();
    }
}