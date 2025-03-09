#include "Config.h"
#include <fstream>
#include <stdexcept>
#include <spdlog/spdlog.h>

Json::Value Config::_configData;

void Config::load(const std::string& configFilePath) {
    std::ifstream configFile(configFilePath, std::ifstream::binary);
    if (!configFile.is_open()) {
        throw std::runtime_error("Unable to open config file: " + configFilePath);
    }

    configFile >> _configData;
    spdlog::info("Configuration loaded from file: {}", configFilePath);
}

std::string Config::getDatabaseConnectionString() {
    return _configData["database"]["connection_string"].asString();
}

std::string Config::getApiBaseUrl() {
    return _configData["api"]["base_url"].asString();
}

uint64_t Config::getApiReconnectTime() {
    return _configData["api"]["update_time"].asUInt64();
}