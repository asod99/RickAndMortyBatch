#ifndef CHARACTERPROCESSOR_H
#define CHARACTERPROCESSOR_H

#include <json/json.h>
#include "DatabaseManager.h"

/**
 * @class CharacterProcessor
 * @brief Class to process character data from the API and store it in the database.
 */
class CharacterProcessor {
public:
    /**
     * @brief Constructs a CharacterProcessor with a reference to a DatabaseManager.
     * @param dbManager Reference to the DatabaseManager for database operations.
     */
    explicit CharacterProcessor(DatabaseManager& dbManager);

    /**
     * @brief Processes character data and stores it in the database.
     * @param data The JSON data containing character information.
     */
    void process(const Json::Value& data);

    /**
     * @brief Extracts the episode ID from a given episode URL.
     * @param episodeUrl The URL of the episode.
     * @return The extracted episode ID.
     */
    int extractEpisodeIdFromUrl(const std::string& episodeUrl);

private:
    DatabaseManager& dbManager; ///< Reference to the DatabaseManager for handling database operations.
};

#endif // CHARACTERPROCESSOR_H