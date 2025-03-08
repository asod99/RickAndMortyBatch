#include "EpisodeProcessor.h"
#include <spdlog/spdlog.h>

EpisodeProcessor::EpisodeProcessor(DatabaseManager& dbManager)
    : dbManager(dbManager) {
    spdlog::info("EpisodeProcessor initialized.");
}

void EpisodeProcessor::process(const Json::Value& data) {
    spdlog::info("Processing episode data.");

    for (const auto& episode : data) {
        int id = episode["id"].asInt();
        std::string name = episode["name"].asString();
        std::string airDate = episode["air_date"].asString();
        std::string episodeCode = episode["episode"].asString();
        std::string url = episode["url"].asString();
        std::string created = episode["created"].asString();

        dbManager.insertEpisode(id, name, airDate, episodeCode, url, created);
    }
}