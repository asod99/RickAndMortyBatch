#include "BatchProcessor.h"
#include <spdlog/spdlog.h>
#include <stdexcept>

BatchProcessor::BatchProcessor(ApiClient& apiClient) : apiClient(apiClient) {
    spdlog::info("BatchProcessor initialized.");
}

void BatchProcessor::execute() {
    spdlog::info("Starting batch processing for all resources.");

    try {
        processResource("character");
        processResource("location");
        processResource("episode");
    } catch (const std::exception& e) {
        spdlog::error("Batch processing failed: {}", e.what());
    }

    spdlog::info("Batch processing completed.");
}

void BatchProcessor::setFilters(const std::string& resource, const std::unordered_map<std::string, std::string>& filters) {
    resourceFilters[resource] = filters;
    spdlog::info("Filters set for resource {}: {}", resource, filters.size());
}

void BatchProcessor::processResource(const std::string& resource) {
    spdlog::info("Processing resource: {}", resource);
    int page = 1;
    bool hasMorePages = true;

    const auto& filters = resourceFilters.find(resource) != resourceFilters.end() ? resourceFilters[resource] : std::unordered_map<std::string, std::string>{};

    while (hasMorePages) {
        try {
            Json::Value data = apiClient.getResource(resource, page, filters);
            const Json::Value& results = data["results"];
            if (results.empty()) {
                spdlog::info("No more results for resource: {}", resource);
                break;
            }

            if (resource == "character") {
                CharacterProcessor::process(results);
            } else if (resource == "location") {
                LocationProcessor::process(results);
            } else if (resource == "episode") {
                EpisodeProcessor::process(results);
            }

            ++page;
            hasMorePages = !data["info"]["next"].isNull();
        } catch (const std::exception& e) {
            spdlog::error("Failed to process resource {}: {}", resource, e.what());
            hasMorePages = false;
        }
    }
}