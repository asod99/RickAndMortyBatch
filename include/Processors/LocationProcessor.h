#ifndef LOCATIONPROCESSOR_H
#define LOCATIONPROCESSOR_H

#include <json/json.h>
#include "DatabaseManager.h"

/**
 * @class LocationProcessor
 * @brief Class to process location data from the API.
 */
class LocationProcessor {
public:
    /**
     * @brief Constructs a LocationProcessor with a reference to a DatabaseManager.
     * @param dbManager Reference to the DatabaseManager for database operations.
     */
    explicit LocationProcessor(DatabaseManager& dbManager);

    /**
     * @brief Processes location data and stores it in the database.
     * @param data The JSON data containing location information.
     */
    void process(const Json::Value& data);
private:
    /**
     * @brief Extracts the character ID from a given character URL.
     * @param episodeUrl The URL of the character.
     * @return The extracted character ID.
     */
    int extractCharacterIdFromUrl(const std::string& characterUrl);

    DatabaseManager& _dbManager; ///< Reference to the DatabaseManager.
};

#endif // LOCATIONPROCESSOR_H