#include "BatchProcessor.h"
#include <spdlog/spdlog.h>
#include <stdexcept>
#include "Logger.h"

BatchProcessor::BatchProcessor(ApiClient& apiClient, DatabaseManager& dbManager) : _apiClient(apiClient), _dbmanager(dbManager) {
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

    const auto& filters = _resourceFilters.find(resource) != _resourceFilters.end() ? _resourceFilters[resource] : std::unordered_map<std::string, std::string>{};

    while (hasMorePages) {
        try {
            spdlog::debug("Fetching page {} for resource: {}", page, resource);
            Json::Value data = _apiClient.getResource(resource, page, filters);
            const Json::Value& results = data["results"];
            if (results.empty()) {
                spdlog::info("No more results for resource: {}", resource);
                break;
            }

            spdlog::info("Processing {} results for resource: {}", results.size(), resource);
            if (resource == "episode") {
                _episodeProcessor->process(results);
            } else if (resource == "location") {
                _locationProcessor->process(results);
            } else if (resource == "character") {
                _characterProcessor->process(results);
            }

            ++page;
            hasMorePages = !data["info"]["next"].isNull();
            if (hasMorePages) {
                spdlog::debug("More pages available for resource: {}. Moving to page {}", resource, page);
            } else {
                spdlog::info("No more pages for resource: {}", resource);
            }
        } catch (const std::exception& e) {
            spdlog::error("Failed to process resource {}: {}", resource, e.what());
            hasMorePages = false;
        }
        Logger::checkLogSizeAndReset();
    }
}