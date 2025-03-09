#include "Config.h"
#include <spdlog/spdlog.h>
#include "ApiClient.h"
#include "Logger.h"
#include "Menu.h"
#include <iostream>
#include "Colors.h"

int main() {
    try{

        Logger::setupNullLogger();

        Color::initialize();

     
        std::cout << Color::cyan << "=====================================" << Color::reset << std::endl;
        std::cout << Color::magenta << "********** " << Color::reset << std::endl;
        Color::printAnimatedText("WELCOME TO RICK AND MORTY BATCH APP", Color::green);
        std::cout << Color::magenta << "**********" << Color::reset << std::endl;
        std::cout << Color::cyan << "=====================================" << Color::reset << std::endl;

        //Show Welcome Menu
        showConfigurationMenu();

    } catch (const std::exception& e) {
        spdlog::error("An error occurred: {}", e.what());
        return EXIT_FAILURE;
    }
    std::quick_exit(EXIT_SUCCESS);
}