#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <json/json.h>

/**
 * @class Config
 * @brief Singleton class to manage application configuration.
 */
class Config {
public:
    /**
     * @brief Loads configuration from a JSON file.
     * @param configFilePath Path to the configuration file.
     */
    static void load(const std::string& configFilePath);

    /**
     * @brief Retrieves the database connection string.
     * @return Database connection string.
     */
    static std::string getDatabaseConnectionString();

    /**
     * @brief Retrieves the base URL for the API.
     * @return Base URL for the API.
     */
    static std::string getApiBaseUrl();

private:
    static Json::Value configData; ///< JSON object to store configuration data.
};

#endif // CONFIG_H