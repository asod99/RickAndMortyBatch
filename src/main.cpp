#include "Config.h"
#include <spdlog/spdlog.h>
#include "ApiClient.h"
#include "Logger.h"
#include "Menu.h"
#include <iostream>

int main() {
    try{

        Logger::setupNullLogger();

        std::cout << "WELCOME TO RICK AND MORTY BATCH APP" << std::endl;

        //Show Welcome Menu
        showConfigurationMenu();

    } catch (const std::exception& e) {
        spdlog::error("An error occurred: {}", e.what());
        return EXIT_FAILURE;
    }
    std::quick_exit(EXIT_SUCCESS);
}