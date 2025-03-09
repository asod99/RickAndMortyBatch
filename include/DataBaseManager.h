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

    // Evitar copia y asignación
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    // Método estático para obtener la instancia única
    static DatabaseManager& getInstance();

    /**
     * @brief Constructs a DatabaseManager and establishes a connection.
     * @param connectionString Connection string for the database.
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


    void updateconnectionStringAndConnect(const std::string& newconnectionString);

    void createDataBaseIfNotExist();

    void insertCharacterEpisode(int characterId, int episodeId);

    void createTablesIfNotExist();

    void insertCharacterLocation(int characterId, int locationId);

    void setHost(const std::string& h) { host = h; }
    void setUser(const std::string& u) { user = u; }
    void setPort(const std::string& p) { port = p; }
    void setPassword(const std::string& pwd) { password = pwd; }

    std::string getHost() const { return host; }
    std::string getUser() const { return user; }
    std::string getPort() const { return port; }

    void connectToDatabase();
    
    bool isConnected();

private:

    std::map<std::string, std::string> parseConnectionString(const std::string& connectionString);

    std::string host;
    std::string user;
    std::string database;
    std::string port;
    std::string password;
    static std::unique_ptr<DatabaseManager> instance;
    std::unique_ptr<pqxx::connection> conn; ///< Database connection.
};

#endif // DATABASEMANAGER_H