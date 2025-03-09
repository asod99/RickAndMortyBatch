#include "Config.h"
#include <spdlog/spdlog.h>
#include "ApiClient.h"
#include "Logger.h"
#include "DatabaseManager.h"
#include "CharacterProcessor.h"
#include "LocationProcessor.h"
#include "EpisodeProcessor.h"
#include "BatchProcessor.h"
#include "Menu.h"
#include <iostream>

int main() {
    try{

        Logger::setupNullLogger();

        std::cout << "WELCOME TO RICK AND MORTY BATCH APP" << std::endl;

        //Show Welcome Menu
        showConfigurationMenu();

        // Show menu
        showMenu();

        // Log application end
        spdlog::info("Application finished.");

    } catch (const std::exception& e) {
        spdlog::error("An error occurred: {}", e.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}