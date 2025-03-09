#include "menu.h"
#include "Config.h"
#include <iostream>
#include <string>
#include "LoggerMenu.h"
#include "DataBaseMenu.h"

void loadDataFromApi() {
    std::cout << "Loading data from API...\n";
    DatabaseManager& dbManager = DatabaseManager::getInstance();
    if(!dbManager.isConnected())
    {
        std::cout << "La base de datos no está conectada\n"; 
        showMenu();
    }
    try {
        BatchProcessor batchProcessor(ApiClient::getInstance(), DatabaseManager::getInstance());
        batchProcessor.execute();

        std::cout << "Data loaded successfully.\n";
    } catch (const std::exception& e) {
        std::cerr << "Error inesperado al cargar los datos: " << e.what() << std::endl;
    }
}

void showMenu() {
    int choice;
    DatabaseManager& dbManager = DatabaseManager::getInstance();
    DatabaseMenu dbMenu(dbManager.getConn());
    do {
        std::cout << "\n--- Rick and Morty Data Manager ---\n";
        std::cout << "1. Load data from API\n";
        std::cout << "2. Filter and search data for database\n";
        std::cout << "3. Show Configuration Menu\n";
        std::cout << "4. Exit\n";
        std::cout << "Choose an option: ";
        std::cin >> choice;
        switch (choice) {
            case 1:
                loadDataFromApi();
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
    } while (choice != 3);
}

void showConfigurationMenu() {
    int choice;
    std::string configFile;
    std::string baseUrl;
    do {
        std::cout << "\n--- Configuration Menu ---\n";
        std::cout << "1. Load configuration from file\n";
        std::cout << "2. Configure API url\n";
        std::cout << "3. Configure Data Base Parameters\n";
        std::cout << "4. Logger\n";
        std::cout << "5. Continue to main menu\n";
        std::cout << "Choose an option: ";
        std::cin >> choice;
        switch (choice) {
            case 1:
                std::cout << "Introduce el nombre del archivo de configuración: ";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // 🔹 Limpia el buffer
                std::getline(std::cin, configFile);
                try{
                    Config::load(configFile);
                    std::cout << "Configuration loaded from file.\n";
                    ApiClient::getInstance().updateBaseUrl(Config::getApiBaseUrl());
                    DatabaseManager::getInstance().updateconnectionStringAndConnect(Config::getDatabaseConnectionString());
                   
                }catch (const pqxx::broken_connection& e) {
                    std::cerr << "Error: No se puede conectar a la base de datos. Verifique la configuración.\n";
                    std::cerr << "Detalle del error: " << e.what() << std::endl;
                    showConfigurationMenu();
                }
                
                break;
            case 2:
            {
                std::cout << "URL de la API Actual: " << ApiClient::getInstance().getBaseUrl() << "\n";
                std::cout << "Introduce la nueva URL de la API (o deja vacío para cancelar): ";
                
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // 🔹 Limpia el buffer
                std::string newBaseUrl;
                std::getline(std::cin, newBaseUrl);
                
                if (newBaseUrl.empty()) {
                    std::cout << "Operación cancelada. La URL no ha cambiado.\n";
                } else {
                    ApiClient::getInstance().updateBaseUrl(newBaseUrl);
                    std::cout << "URL actualizada a: " << ApiClient::getInstance().getBaseUrl() << "\n";
                }
                break;
            }
            case 3:
                updateDatabaseConfig();
                break;
            case 4:
                showLoggerMenu();
                break;
            case 5:
                showMenu();
                break;
            default:
                std::cout << "Invalid option. Please try again.\n";
        }
    } while (choice != 5);
}

void updateDatabaseConfig() {

    DatabaseManager& dbManager = DatabaseManager::getInstance();

    while (true) {
        std::cout << "\n--- Configuración Actual de la Base de Datos ---\n";
        std::cout << "1. Host: " << dbManager.getHost() << "\n";
        std::cout << "2. Usuario: " << dbManager.getUser() << "\n";
        std::cout << "3. Puerto: " << dbManager.getPort() << "\n";
        std::cout << "4. Contraseña: [OCULTA]\n";
        std::cout << "5. Salir sin guardar\n";
        std::cout << "6. Guardar cambios y salir\n";
        std::cout << "Seleccione un parámetro para modificar (1-6): ";

        int opcion;
        std::cin >> opcion;
        std::cin.ignore();  // Limpiar el buffer

        std::string newValue;
        if (opcion >= 1 && opcion <= 4) {
            std::cout << "Ingrese el nuevo valor: ";
            std::getline(std::cin, newValue);
        }

        switch (opcion) {
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
                try{
                    dbManager.getInstance().connectToDatabase();
                    std::cout << "Conexión actualizada correctamente.\n";
                    return;
                }catch (const pqxx::broken_connection& e) {
                    std::cerr << "Error: No se puede conectar a la base de datos. Verifique la configuración.\n";
                    std::cerr << "Detalle del error: " << e.what() << std::endl;
                    return;
                }
            default:
                std::cout << "Opción no válida. Inténtelo de nuevo.\n";
        }
    }
}
