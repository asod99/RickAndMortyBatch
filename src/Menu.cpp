#include "menu.h"
#include <iostream>
#include <string>

void loadDataFromApi(BatchProcessor& batchProcessor) {
    std::cout << "Loading data from API...\n";
    batchProcessor.execute();
    std::cout << "Data loaded successfully.\n";
}

void updateDatabase(DatabaseManager& dbManager) {
    std::cout << "Updating database...\n";
    // Logic to update the database, possibly by re-fetching data from the API
    // and applying updates where necessary
    std::cout << "Database updated successfully.\n";
}

void viewStoredData(DatabaseManager& dbManager) {
    std::cout << "Stored data:\n";
    // Logic to query and display all data from the database
    // Example: dbManager.queryAllCharacters();
}

void filterAndSearchData(DatabaseManager& dbManager) {
    std::string filter;
    std::cout << "Enter filter criteria (e.g., status=Alive): ";
    std::cin.ignore();
    std::getline(std::cin, filter);
    // Logic to filter and search data in the database using the filter
    // Example: dbManager.filterCharacters(filter);
}

void sortData(DatabaseManager& dbManager) {
    std::cout << "Sorting data...\n";
    // Logic to sort data in the database
    // Example: dbManager.sortCharactersBy("name");
}

void showStatistics(DatabaseManager& dbManager) {
    std::cout << "Statistics:\n";
    // Logic to compute and display statistics about the data
    // Example: dbManager.showCharacterStatistics();
}

void showMenu(BatchProcessor& batchProcessor, DatabaseManager& dbManager) {
    int choice;
    do {
        std::cout << "\n--- Rick and Morty Data Manager ---\n";
        std::cout << "1. Load data from API\n";
        std::cout << "2. Update database\n";
        std::cout << "3. View stored data\n";
        std::cout << "4. Filter and search data\n";
        std::cout << "5. Sort data\n";
        std::cout << "6. Show statistics\n";
        std::cout << "7. Exit\n";
        std::cout << "Choose an option: ";
        std::cin >> choice;
        switch (choice) {
            case 1:
                loadDataFromApi(batchProcessor);
                break;
            case 2:
                updateDatabase(dbManager);
                break;
            case 3:
                viewStoredData(dbManager);
                break;
            case 4:
                filterAndSearchData(dbManager);
                break;
            case 5:
                sortData(dbManager);
                break;
            case 6:
                showStatistics(dbManager);
                break;
            case 7:
                std::cout << "Exiting...\n";
                break;
            default:
                std::cout << "Invalid option. Please try again.\n";
        }
    } while (choice != 7);
}