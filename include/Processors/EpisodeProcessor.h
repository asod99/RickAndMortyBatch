#ifndef EPISODEPROCESSOR_H
#define EPISODEPROCESSOR_H

#include <json/json.h>
#include "DatabaseManager.h"

/**
 * @class EpisodeProcessor
 * @brief Class to process episode data from the API.
 */
class EpisodeProcessor {
public:
    /**
     * @brief Constructs an EpisodeProcessor with a reference to a DatabaseManager.
     * @param dbManager Reference to the DatabaseManager for database operations.
     */
    explicit EpisodeProcessor(DatabaseManager& dbManager);

    /**
     * @brief Processes episode data and stores it in the database.
     * @param data The JSON data containing episode information.
     */
    void process(const Json::Value& data);

private:
    DatabaseManager& _dbManager; ///< Reference to the DatabaseManager.
};

#endif // EPISODEPROCESSOR_H