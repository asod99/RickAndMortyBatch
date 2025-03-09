#include "menu.h"
#include "Config.h"
#include <iostream>
#include <string>
#include "LoggerMenu.h"
#include "DataBaseMenu.h"
#include "APILoader.h"
#include <spdlog/spdlog.h>

void showMenu() {
    int choice;
    DatabaseMenu dbMenu;
    do {
        std::cout << "\n--- Rick and Morty Data Manager ---\n";
        std::cout << "1. Load data from API\n";
        std::cout << "2. Filter and search data from database\n";
        std::cout << "3. Show Configuration Menu\n";
        std::cout << "4. Exit\n";
        std::cout << "Choose an option: ";
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cout << "Invalid input. Please enter a number.\n";
            std::cin.clear();  // Limpiamos el estado de error
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Limpiamos el buffer de entrada
            continue;
        }

        switch (choice) {
            case 1:
                std::cout << "Loading Data...Please wait this can take some time...\n";
                ApiLoader::getInstance().loadDataManually();
                break;
            case 2:
                dbMenu.dataBaseMenu();
                break;
            case 3:
                showConfigurationMenu();
                break;
            case 4:
                std::cout << "Exiting...\n";
                break;
            default:
                std::cout << "Invalid option. Please try again.\n";
        }
    } while (choice != 4);
}

void showConfigurationMenu() {
    int choice;
    std::string configFile;
    std::string baseUrl;
    do {
        std::cout << "\n--- Configuration Menu ---\n";
        std::cout << "1. Load configuration from file\n";
        std::cout << "2. Configure API URL\n";
        std::cout << "3. Configure Database Parameters\n";
        std::cout << "4. Configure Time Batch Reconnection\n";
        std::cout << "5. Logger\n";
        std::cout << "6. Continue to main menu\n";
        std::cout << "Choose an option: ";
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cout << "Invalid input. Please enter a number.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
            case 1:
                std::cout << "Enter the configuration file name: ";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::getline(std::cin, configFile);
                if (configFile.empty()) {
                    std::cout << "Empty name.\n";
                    spdlog::warn("Empty configuration file name provided.");
                    break;
                }
                try {
                    Config::load(configFile);
                    std::cout << "Configuration loaded from file.\n";
                    ApiClient::getInstance().updateBaseUrl(Config::getApiBaseUrl());
                    DatabaseManager::getInstance().updateconnectionStringAndConnect(Config::getDatabaseConnectionString());
                    if (Config::getApiReconnectTime() != 0) {
                        ApiLoader::getInstance().updateInterval(Config::getApiReconnectTime());
                    }
                } catch (const pqxx::broken_connection& e) {
                    std::cerr << "Error: Cannot connect to the database. Check the configuration.\n";
                    std::cerr << "Error detail: " << e.what() << std::endl;
                    spdlog::error("Database connection error: {}", e.what());
                } catch (const std::runtime_error& e) {
                    std::cerr << "Error: " << e.what() << std::endl;
                    spdlog::error("Runtime error: {}", e.what());
                }
                break;
            case 2: {
                std::cout << "Current API URL: " << ApiClient::getInstance().getBaseUrl() << "\n";
                std::cout << "Enter the new API URL (or leave empty to cancel): ";
                
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::string newBaseUrl;
                std::getline(std::cin, newBaseUrl);
                
                if (newBaseUrl.empty()) {
                    std::cout << "Operation canceled. The URL has not changed.\n";
                } else {
                    ApiClient::getInstance().updateBaseUrl(newBaseUrl);
                    std::cout << "URL updated to: " << ApiClient::getInstance().getBaseUrl() << "\n";
                }
                break;
            }
            case 3:
                updateDatabaseConfig();
                break;
            case 4: {
                std::cout << "Current reconnection time: " << ApiLoader::getInstance().getInterval() << "\n";
                std::cout << "Enter the new reconnection time (or leave empty to cancel): ";
                
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::string newTime;
                std::getline(std::cin, newTime);
                
                if (newTime.empty()) {
                    std::cout << "Operation canceled. The time has not changed.\n";
                } else {
                    std::stringstream ss(newTime);
                    int newInterval;
                    if (ss >> newInterval && ss.eof() && newInterval > 0) {
                        ApiLoader::getInstance().updateInterval(newInterval);
                        std::cout << "Time updated to: " << ApiLoader::getInstance().getInterval() << "\n";
                    } else {
                        std::cout << "Error: Please enter a valid integer greater than zero.\n";
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
                std::cout << "Invalid option. Please try again.\n";
        }
    } while (choice != 6);
}

void updateDatabaseConfig() {
    DatabaseManager& dbManager = DatabaseManager::getInstance();

    while (true) {
        std::cout << "\n--- Current Database Configuration ---\n";
        std::cout << "1. Host: " << dbManager.getHost() << "\n";
        std::cout << "2. User: " << dbManager.getUser() << "\n";
        std::cout << "3. Port: " << dbManager.getPort() << "\n";
        std::cout << "4. Password: [HIDDEN]\n";
        std::cout << "5. Exit without saving\n";
        std::cout << "6. Save changes and exit\n";
        std::cout << "Select a parameter to modify (1-6): ";

        int option;
        std::cin >> option;
        std::cin.ignore();

        std::string newValue;
        if (option >= 1 && option <= 4) {
            std::cout << "Enter the new value: ";
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
                dbManager.setPassword(newValue);
                break;
            case 5:
                std::cout << "Saliendo sin guardar cambios...\n";
                return;
            case 6:
                try {
                    dbManager.getInstance().connectToDatabase();
                    std::cout << "Connection updated successfully.\n";
                    return;
                } catch (const pqxx::broken_connection& e) {
                    std::cerr << "Error: Cannot connect to the database. Check the configuration.\n";
                    std::cerr << "Error detail: " << e.what() << std::endl;
                    return;
                }
            default:
                std::cout << "Invalid option. Please try again.\n";
        }
    }
}