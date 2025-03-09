#include "menu.h"
#include "Config.h"
#include <iostream>
#include <string>
#include "LoggerMenu.h"
#include "DataBaseMenu.h"
#include "APILoader.h"
#include <spdlog/spdlog.h>
#include "Colors.h"

void showMenu() {
    int choice;
    DatabaseMenu dbMenu;
    do {
        Color::printAnimatedText("\n--- Rick and Morty Data Manager ---", Color::cyan);
        std::cout << Color::yellow << "1. " << Color::reset << Color::green << "Load data from API" << Color::reset << std::endl;
        std::cout << Color::yellow << "2. " << Color::reset << Color::green << "Filter and search data from database" << Color::reset << std::endl;
        std::cout << Color::yellow << "3. " << Color::reset << Color::green << "Show Configuration Menu" << Color::reset << std::endl;
        std::cout << Color::yellow << "4. " << Color::reset << Color::green << "Exit" << Color::reset << std::endl;
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
                Color::printAnimatedText("Loading Data...Please wait this can take some time...", Color::cyan);
                ApiLoader::getInstance().loadDataManually();
                Color::printAnimatedText("Data Succesfully Loaded", Color::yellow);
                break;
            case 2:
                dbMenu.dataBaseMenu();
                break;
            case 3:
                showConfigurationMenu();
                break;
            case 4:
                Color::printAnimatedText("Exiting...", Color::red);
                break;
            default:
                std::cout << Color::red << "Invalid option. Please try again." << Color::reset << std::endl;
        }
    } while (choice != 4);
}

void showConfigurationMenu() {
    int choice;
    std::string configFile;
    std::string baseUrl;
    do {
        Color::printAnimatedText("\n--- Configuration Menu ---", Color::cyan);
        std::cout << Color::yellow << "1. " << Color::reset << Color::green << "Load configuration from file" << Color::reset << std::endl;
        std::cout << Color::yellow << "2. " << Color::reset << Color::green << "Configure API URL" << Color::reset << std::endl;
        std::cout << Color::yellow << "3. " << Color::reset << Color::green << "Configure Database Parameters" << Color::reset << std::endl;
        std::cout << Color::yellow << "4. " << Color::reset << Color::green << "Configure Time Batch Reconnection" << Color::reset << std::endl;
        std::cout << Color::yellow << "5. " << Color::reset << Color::green << "Logger" << Color::reset << std::endl;
        std::cout << Color::yellow << "6. " << Color::reset << Color::green << "Continue to main menu" << Color::reset << std::endl;
        std::cout << Color::magenta << "Choose an option: " << Color::reset;
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cout << Color::red << "Invalid input. Please enter a number." << Color::reset << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
            case 1:
                std::cout << Color::cyan << "Enter the configuration file name: " << Color::reset;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::getline(std::cin, configFile);
                if (configFile.empty()) {
                    std::cout << Color::red << "Empty name." << Color::reset << std::endl;
                    spdlog::warn("Empty configuration file name provided.");
                    break;
                }
                try {
                    Config::load(configFile);
                    std::cout << Color::green << "Configuration loaded from file." << Color::reset << std::endl;
                    ApiClient::getInstance().updateBaseUrl(Config::getApiBaseUrl());
                    DatabaseManager::getInstance().updateconnectionStringAndConnect(Config::getDatabaseConnectionString());
                    if (Config::getApiReconnectTime() != 0) {
                        ApiLoader::getInstance().updateInterval(Config::getApiReconnectTime());
                    }
                } catch (const pqxx::broken_connection& e) {
                    std::cerr << Color::red << "Error: Cannot connect to the database. Check the configuration." << Color::reset << std::endl;
                    std::cerr << Color::red << "Error detail: " << e.what() << Color::reset << std::endl;
                    spdlog::error("Database connection error: {}", e.what());
                } catch (const std::runtime_error& e) {
                    std::cerr << Color::red << "Error: " << e.what() << Color::reset << std::endl;
                    spdlog::error("Runtime error: {}", e.what());
                }
                break;
            case 2: {
                std::cout << Color::cyan << "Current API URL: " << ApiClient::getInstance().getBaseUrl() << Color::reset << std::endl;
                std::cout << Color::cyan << "Enter the new API URL (or leave empty to cancel): " << Color::reset;
                
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::string newBaseUrl;
                std::getline(std::cin, newBaseUrl);
                
                if (newBaseUrl.empty()) {
                    std::cout << Color::yellow << "Operation canceled. The URL has not changed." << Color::reset << std::endl;
                } else {
                    ApiClient::getInstance().updateBaseUrl(newBaseUrl);
                    std::cout << Color::green << "URL updated to: " << ApiClient::getInstance().getBaseUrl() << Color::reset << std::endl;
                }
                break;
            }
            case 3:
                updateDatabaseConfig();
                break;
            case 4: {
                std::cout << Color::cyan << "Current reconnection time: " << ApiLoader::getInstance().getInterval() << Color::reset << std::endl;
                std::cout << Color::cyan << "Enter the new reconnection time (or leave empty to cancel): " << Color::reset;
                
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::string newTime;
                std::getline(std::cin, newTime);
                
                if (newTime.empty()) {
                    std::cout << Color::yellow << "Operation canceled. The time has not changed." << Color::reset << std::endl;
                } else {
                    std::stringstream ss(newTime);
                    int newInterval;
                    if (ss >> newInterval && ss.eof() && newInterval > 0) {
                        ApiLoader::getInstance().updateInterval(newInterval);
                        std::cout << Color::green << "Time updated to: " << ApiLoader::getInstance().getInterval() << Color::reset << std::endl;
                    } else {
                        std::cout << Color::red << "Error: Please enter a valid integer greater than zero." << Color::reset << std::endl;
                    }
                }
                break;
            }
            case 5:
                showLoggerMenu();
                break;
            case 6:
                showMenu();
                break;
            default:
                std::cout << Color::red << "Invalid option. Please try again." << Color::reset << std::endl;
        }
    } while (choice != 6);
}

void updateDatabaseConfig() {
    DatabaseManager& dbManager = DatabaseManager::getInstance();

    while (true) {
        Color::printAnimatedText("\n--- Current Database Configuration ---", Color::cyan);
        std::cout << Color::yellow << "1. " << Color::reset << "Host: " << dbManager.getHost() << std::endl;
        std::cout << Color::yellow << "2. " << Color::reset << "User: " << dbManager.getUser() << std::endl;
        std::cout << Color::yellow << "3. " << Color::reset << "Port: " << dbManager.getPort() << std::endl;
        std::cout << Color::yellow << "4. " << Color::reset << "DataBase: " << dbManager.getBaseName() << std::endl;
        std::cout << Color::yellow << "5. " << Color::reset << "Password: [HIDDEN]" << std::endl;
        std::cout << Color::yellow << "6. " << Color::reset << "Exit without saving" << std::endl;
        std::cout << Color::yellow << "7. " << Color::reset << "Save changes and exit" << std::endl;
        std::cout << Color::magenta << "Select a parameter to modify (1-6): " << Color::reset;

        int option;
        std::cin >> option;
        std::cin.ignore();

        std::string newValue;
        if (option >= 1 && option <= 4) {
            std::cout << Color::cyan << "Enter the new value: " << Color::reset;
            std::getline(std::cin, newValue);
        }

        switch (option) {
            case 1:
                dbManager.setHost(newValue);
                break;
            case 2:
                dbManager.setUser(newValue);
                break;
            case 3:
                dbManager.setPort(newValue);
                break;
            case 4:
                dbManager.setBaseName(newValue);
                break;
            case 5:
                dbManager.setPassword(newValue);
                break;
            case 6:
                std::cout << Color::yellow << "Saliendo sin guardar cambios..." << Color::reset << std::endl;
                return;
            case 7:
                try {
                    dbManager.getInstance().connectToDatabase();
                    std::cout << Color::green << "Connection updated successfully." << Color::reset << std::endl;
                    return;
                } catch (const pqxx::broken_connection& e) {
                    std::cerr << Color::red << "Error: Cannot connect to the database. Check the configuration." << Color::reset << std::endl;
                    std::cerr << Color::red << "Error detail: " << e.what() << Color::reset << std::endl;
                    return;
                }
            default:
                std::cout << Color::red << "Invalid option. Please try again." << Color::reset << std::endl;
        }
    }
}