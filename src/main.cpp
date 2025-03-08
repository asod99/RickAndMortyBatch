#include "Config.h"
#include "Logger.h"
#include "ApiClient.h"
#include "DatabaseManager.h"
#include "CharacterProcessor.h"
#include "LocationProcessor.h"
#include "EpisodeProcessor.h"
#include "BatchProcessor.h"
#include "Menu.h"
#include <iostream>

int main() {
    try {
        // Load configuration
        Config::load("config/config.json");

        // Initialize logger
        Logger::init(Config::getLogFilePath());

        // Log application start
        spdlog::info("Application started.");

        // Initialize database manager
        DatabaseManager dbManager(Config::getDatabaseConnectionString());

        // Initialize API client
        ApiClient apiClient(Config::getApiBaseUrl());

        // Initialize processors
        CharacterProcessor charProcessor(dbManager);
        LocationProcessor locProcessor(dbManager);
        EpisodeProcessor epiProcessor(dbManager);

        // Initialize batch processor
        BatchProcessor batchProcessor(apiClient);

        // Show menu
        showMenu(batchProcessor, dbManager);

        // Log application end
        spdlog::info("Application finished.");

    } catch (const std::exception& e) {
        spdlog::error("An error occurred: {}", e.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}