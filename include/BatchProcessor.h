#ifndef BATCHPROCESSOR_H
#define BATCHPROCESSOR_H

#include <string>
#include <unordered_map>
#include "ApiClient.h"
#include "CharacterProcessor.h"
#include "LocationProcessor.h"
#include "EpisodeProcessor.h"

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
    explicit BatchProcessor(ApiClient& apiClient);

    /**
     * @brief Executes the batch processing for all resources.
     */
    void execute();

    /**
     * @brief Sets filters for resource processing.
     * @param resource The resource type to apply filters (e.g., "character").
     * @param filters Filters to apply for the specified resource.
     */
    void setFilters(const std::string& resource, const std::unordered_map<std::string, std::string>& filters);

private:
    ApiClient& apiClient; ///< Reference to the ApiClient.
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> resourceFilters; ///< Filters for each resource.

    /**
     * @brief Processes a specific resource type.
     * @param resource The resource type to process (e.g., "character", "location", "episode").
     */
    void processResource(const std::string& resource);
};

#endif // BATCHPROCESSOR_H