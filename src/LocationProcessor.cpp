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

        dbManager.insertLocation(id, name, type, dimension,url, created);

        // Process characters for this location
        const Json::Value& characters = location["residents"];
        for (const auto& characterUrl : characters) {
            std::string characterUrlString = characterUrl.asString();
            int characterId = extractCharacterIdFromUrl(characterUrlString);
            if (characterId != -1) {
                dbManager.insertCharacterLocation(characterId, id);
            }
        }
    }
}

int LocationProcessor::extractCharacterIdFromUrl(const std::string& characterUrl) {
    size_t lastSlashPos = characterUrl.find_last_of('/');
    if (lastSlashPos != std::string::npos) {
        return std::stoi(characterUrl.substr(lastSlashPos + 1));
    }
    return -1;
}