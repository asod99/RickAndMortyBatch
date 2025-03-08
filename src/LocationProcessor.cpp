#include "LocationProcessor.h"
#include <spdlog/spdlog.h>

LocationProcessor::LocationProcessor(DatabaseManager& dbManager)
    : dbManager(dbManager) {
    spdlog::info("LocationProcessor initialized.");
}

void LocationProcessor::process(const Json::Value& data) {
    spdlog::info("Processing location data.");

    for (const auto& location : data) {
        int id = location["id"].asInt();
        std::string name = location["name"].asString();
        std::string type = location["type"].asString();
        std::string dimension = location["dimension"].asString();
        std::string url = location["url"].asString();
        std::string created = location["created"].asString();

        dbManager.insertLocation(id, name, type, dimension, url, created);
    }
}