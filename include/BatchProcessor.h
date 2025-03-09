#ifndef BATCHPROCESSOR_H
#define BATCHPROCESSOR_H

#include <string>
#include <unordered_map>
#include "ApiClient.h"
#include "CharacterProcessor.h"
#include "LocationProcessor.h"
#include "EpisodeProcessor.h"
#include "DataBaseManager.h"

/**
 * @class BatchProcessor
 * @brief Class to manage the batch processing of API data.
 */
class BatchProcessor {
public:
    /**
     * @brief Constructs a BatchProcessor.
     * @param apiClient The ApiClient used to fetch data.
     */
    explicit BatchProcessor(ApiClient& apiClient , DatabaseManager& dbManager);

    /**
     * @brief Executes the batch processing for all resources.
     */
    void execute();

private:
    ApiClient& _apiClient; ///< Reference to the ApiClient.
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> resourceFilters; ///< Filters for each resource.

    /**
     * @brief Processes a specific resource type.
     * @param resource The resource type to process (e.g., "character", "location", "episode").
     */
    void processResource(const std::string& resource);

    std::unique_ptr<CharacterProcessor> _characterProcessor;
    std::unique_ptr<LocationProcessor> _locationProcessor;
    std::unique_ptr< EpisodeProcessor> _episodeProcessor;
    DatabaseManager& _dbmanager;
    

};

#endif // BATCHPROCESSOR_H