#include "BatchProcessor.h"
#include <spdlog/spdlog.h>
#include <stdexcept>

BatchProcessor::BatchProcessor(ApiClient& apiClient, DatabaseManager& dbManager) : _apiClient(apiClient), _dbmanager(dbManager)
{
    _characterProcessor = std::make_unique<CharacterProcessor>(dbManager);
    _locationProcessor = std::make_unique<LocationProcessor>(dbManager);
    _episodeProcessor = std::make_unique<EpisodeProcessor>(dbManager);
    spdlog::info("BatchProcessor initialized.");
}

void BatchProcessor::execute() {
    spdlog::info("Starting batch processing for all resources.");

    try {
        processResource("episode");
        processResource("character");
        processResource("location");
    } catch (const std::exception& e) {
        spdlog::error("Batch processing failed: {}", e.what());
    }

    spdlog::info("Batch processing completed.");
}

void BatchProcessor::processResource(const std::string& resource) {
    spdlog::info("Processing resource: {}", resource);
    int page = 1;
    bool hasMorePages = true;

    const auto& filters = resourceFilters.find(resource) != resourceFilters.end() ? resourceFilters[resource] : std::unordered_map<std::string, std::string>{};

    while (hasMorePages) {
        try {
            Json::Value data = _apiClient.getResource(resource, page, filters);
            const Json::Value& results = data["results"];
            if (results.empty()) {
                spdlog::info("No more results for resource: {}", resource);
                break;
            }

            if (resource == "episode") {
                _episodeProcessor->process(results);
            } else if (resource == "location") {
                _locationProcessor->process(results);
            } else if (resource == "character") {
                _characterProcessor->process(results);
            }

            ++page;
            hasMorePages = !data["info"]["next"].isNull();
        } catch (const std::exception& e) {
            spdlog::error("Failed to process resource {}: {}", resource, e.what());
            hasMorePages = false;
        }
    }
}