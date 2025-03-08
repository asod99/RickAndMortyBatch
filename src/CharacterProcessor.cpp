#include "CharacterProcessor.h"
#include <spdlog/spdlog.h>

CharacterProcessor::CharacterProcessor(DatabaseManager& dbManager)
    : dbManager(dbManager) {
    spdlog::info("CharacterProcessor initialized.");
}

void CharacterProcessor::process(const Json::Value& data) {
    spdlog::info("Processing character data.");

    for (const auto& character : data) {
        int id = character["id"].asInt();
        std::string name = character["name"].asString();
        std::string status = character["status"].asString();
        std::string species = character["species"].asString();
        std::string type = character["type"].asString();
        std::string gender = character["gender"].asString();
        std::string originName = character["origin"]["name"].asString();
        std::string originUrl = character["origin"]["url"].asString();
        std::string locationName = character["location"]["name"].asString();
        std::string locationUrl = character["location"]["url"].asString();
        std::string image = character["image"].asString();
        std::string url = character["url"].asString();
        std::string created = character["created"].asString();

        dbManager.insertCharacter(id, name, status, species, type, gender, originName, originUrl, locationName, locationUrl, image, url, created);
    }
}