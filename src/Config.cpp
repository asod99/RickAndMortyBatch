#include "Config.h"
#include <fstream>
#include <stdexcept>
#include <spdlog/spdlog.h>

Json::Value Config::configData;

void Config::load(const std::string& configFilePath) {
    std::ifstream configFile(configFilePath, std::ifstream::binary);
    if (!configFile.is_open()) {
        throw std::runtime_error("Unable to open config file: " + configFilePath);
    }

    configFile >> configData;
    spdlog::info("Configuration loaded from file: {}", configFilePath);
}

std::string Config::getDatabaseConnectionString() {
    return configData["database"]["connection_string"].asString();
}

std::string Config::getLogFilePath() {
    return configData["logging"]["log_file_path"].asString();
}

std::string Config::getApiBaseUrl() {
    return configData["api"]["base_url"].asString();
}