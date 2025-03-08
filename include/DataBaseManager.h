#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <string>
#include <pqxx/pqxx> // PostgreSQL C++ library

/**
 * @class DatabaseManager
 * @brief Class to manage database connections and operations.
 */
class DatabaseManager {
public:
    /**
     * @brief Constructs a DatabaseManager and establishes a connection.
     * @param connectionString Connection string for the database.
     */
    explicit DatabaseManager(const std::string& connectionString);

    /**
     * @brief Inserts a character into the database.
     * @param id Character ID.
     * @param name Character name.
     * @param status Character status.
     * @param species Character species.
     * @param type Character type.
     * @param gender Character gender.
     * @param originName Origin location name.
     * @param originUrl Origin location URL.
     * @param locationName Last known location name.
     * @param locationUrl Last known location URL.
     * @param image Character image URL.
     * @param url Character URL.
     * @param created Creation timestamp.
     */
    void insertCharacter(int id, const std::string& name, const std::string& status,
                         const std::string& species, const std::string& type, const std::string& gender,
                         const std::string& originName, const std::string& originUrl,
                         const std::string& locationName, const std::string& locationUrl,
                         const std::string& image, const std::string& url, const std::string& created);

    /**
     * @brief Inserts a location into the database.
     * @param id Location ID.
     * @param name Location name.
     * @param type Location type.
     * @param dimension Location dimension.
     * @param url Location URL.
     * @param created Creation timestamp.
     */
    void insertLocation(int id, const std::string& name, const std::string& type,
                        const std::string& dimension, const std::string& url, const std::string& created);

    /**
     * @brief Inserts an episode into the database.
     * @param id Episode ID.
     * @param name Episode name.
     * @param airDate Air date of the episode.
     * @param episode Episode code.
     * @param url Episode URL.
     * @param created Creation timestamp.
     */
    void insertEpisode(int id, const std::string& name, const std::string& airDate,
                       const std::string& episode, const std::string& url, const std::string& created);

private:
    pqxx::connection conn; ///< Database connection.
};

#endif // DATABASEMANAGER_H