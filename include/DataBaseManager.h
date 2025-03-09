#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <string>
#include <pqxx/pqxx>
#include <map>
#include <mutex>
#include <memory>

/**
 * @class DatabaseManager
 * @brief Class to manage database connections and operations.
 *
 * This class provides methods to connect to a PostgreSQL database and perform
 * various operations such as inserting characters, locations, and episodes.
 */
class DatabaseManager {
public:
    /**
     * @brief Deleted copy constructor to prevent copying.
     */
    DatabaseManager(const DatabaseManager&) = delete;

    /**
     * @brief Deleted assignment operator to prevent copying.
     */
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    /**
     * @brief Static method to get the singleton instance of DatabaseManager.
     * @return The unique instance of DatabaseManager.
     */
    static DatabaseManager& getInstance();

    /**
     * @brief Constructs a DatabaseManager and establishes a connection.
     */
    explicit DatabaseManager();

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

    /**
     * @brief Updates the connection string and re-establishes the connection.
     * @param newConnectionString The new connection string.
     */
    void updateconnectionStringAndConnect(const std::string& newconnectionString);

    /**
     * @brief Creates the database if it does not exist.
     */
    void createDataBaseIfNotExist();

    /**
     * @brief Inserts a character-episode relationship into the database.
     * @param characterId Character ID.
     * @param episodeId Episode ID.
     */
    void insertCharacterEpisode(int characterId, int episodeId);

    /**
     * @brief Creates necessary tables if they do not exist.
     */
    void createTablesIfNotExist();

    /**
     * @brief Inserts a character-location relationship into the database.
     * @param characterId Character ID.
     * @param locationId Location ID.
     */
    void insertCharacterLocation(int characterId, int locationId);

    /**
     * @brief Sets the host for the database connection.
     * @param h Hostname.
     */
    void setHost(const std::string& h) { host = h; }

    /**
     * @brief Sets the user for the database connection.
     * @param u Username.
     */
    void setUser(const std::string& u) { user = u; }

    /**
     * @brief Sets the port for the database connection.
     * @param p Port number.
     */
    void setPort(const std::string& p) { port = p; }

    /**
     * @brief Sets the password for the database connection.
     * @param pwd Password.
     */
    void setPassword(const std::string& pwd) { password = pwd; }

    /**
     * @brief Gets the host for the database connection.
     * @return Hostname.
     */
    std::string getHost() const { return host; }

    /**
     * @brief Gets the user for the database connection.
     * @return Username.
     */
    std::string getUser() const { return user; }

    /**
     * @brief Gets the port for the database connection.
     * @return Port number.
     */
    std::string getPort() const { return port; }

    /**
     * @brief Connects to the database using the current connection settings.
     */
    void connectToDatabase();
    
    /**
     * @brief Checks if there is an active connection to the database.
     * @return True if connected, false otherwise.
     */
    bool isConnected();

    /**
     * @brief Gets the current database connection.
     * @return Reference to the pqxx::connection object.
     */
    pqxx::connection& getConn() {
        return *conn;
    }

private:
    /**
     * @brief Parses a connection string into its components.
     * @param connectionString The connection string to parse.
     * @return A map of connection components.
     */
    std::map<std::string, std::string> parseConnectionString(const std::string& connectionString);

    std::string host; ///< Hostname for the database connection.
    std::string user; ///< Username for the database connection.
    std::string database; ///< Database name.
    std::string port; ///< Port number for the database connection.
    std::string password; ///< Password for the database connection.
    static std::unique_ptr<DatabaseManager> instance; ///< Singleton instance of DatabaseManager.
    std::mutex dbMutex; ///< Mutex for thread-safe database operations.
    std::unique_ptr<pqxx::connection> conn; ///< Database connection.

    friend class DatabaseMenu; ///< Allows DatabaseMenu to access private members.
};

#endif // DATABASEMANAGER_H