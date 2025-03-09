#include "DatabaseManager.h"
#include <spdlog/spdlog.h>
#include "iostream"
#include <APILoader.h>

std::unique_ptr<DatabaseManager> DatabaseManager::instance = nullptr;

DatabaseManager::DatabaseManager(): host("localhost"), user("usuario"), database("rickandmorty"), port("5432"), password("default_password")
{
    spdlog::info("DatabaseManager initialized and connected to database.");
}

void DatabaseManager::insertCharacter(int id, const std::string& name, const std::string& status,
    const std::string& species, const std::string& type, const std::string& gender,
    const std::string& originName, const std::string& originUrl,
    const std::string& locationName, const std::string& locationUrl,
    const std::string& image, const std::string& url, const std::string& created) {
    try {
        pqxx::work txn(*conn);

        // SQL para insertar o actualizar el personaje en caso de conflicto
        std::string query = R"(
            INSERT INTO characters (id, name, status, species, type, gender, origin_name, origin_url, location_name, location_url, image_url, url, created)
            VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13)
            ON CONFLICT (id) 
            DO UPDATE SET
                name = EXCLUDED.name,
                status = EXCLUDED.status,
                species = EXCLUDED.species,
                type = EXCLUDED.type,
                gender = EXCLUDED.gender,
                origin_name = EXCLUDED.origin_name,
                origin_url = EXCLUDED.origin_url,
                location_name = EXCLUDED.location_name,
                location_url = EXCLUDED.location_url,
                image_url = EXCLUDED.image_url,
                url = EXCLUDED.url,
                created = EXCLUDED.created;
        )";

        txn.exec(query, pqxx::params{id, name, status, species, type, gender, originName, originUrl, locationName, locationUrl, image, url, created});
        txn.commit();
        spdlog::info("Character inserted or updated in database: {}", name);
    } catch (const std::exception& e) {
        spdlog::error("Failed to insert or update character: {}", e.what());
    }
}

void DatabaseManager::insertLocation(int id, const std::string& name, const std::string& type,
    const std::string& dimension, const std::string& url, const std::string& created) {
    try {
        pqxx::work txn(*conn);
        std::string query = R"(
            INSERT INTO locations (id, name, type, dimension, url, created)
            VALUES ($1, $2, $3, $4, $5, $6)
            ON CONFLICT (id) 
            DO UPDATE SET
                name = EXCLUDED.name,
                type = EXCLUDED.type,
                dimension = EXCLUDED.dimension,
                url = EXCLUDED.url,
                created = EXCLUDED.created;
        )";
        txn.exec(query, pqxx::params{id, name, type, dimension, url, created});
        txn.commit();
        spdlog::info("Location inserted or updated in database: {}", name);
    } catch (const std::exception& e) {
        spdlog::error("Failed to insert or update location: {}", e.what());
    }
}



void DatabaseManager::insertEpisode(int id, const std::string& name, const std::string& airDate,
    const std::string& episode, const std::string& url, const std::string& created) {
    try {
        std::lock_guard<std::mutex> lock(dbMutex);
        pqxx::work txn(*conn);
        std::string query = R"(
            INSERT INTO episodes (id, name, air_date, episode, url, created)
            VALUES ($1, $2, $3, $4, $5, $6)
            ON CONFLICT (id) 
            DO UPDATE SET
                name = EXCLUDED.name,
                air_date = EXCLUDED.air_date,
                episode = EXCLUDED.episode,
                url = EXCLUDED.url,
                created = EXCLUDED.created;
        )";
        txn.exec(query, pqxx::params{id, name, airDate, episode, url, created});
        txn.commit();
        spdlog::info("Episode inserted or updated in database: {}", name);
    } catch (const std::exception& e) {
        spdlog::error("Failed to insert or update episode: {}", e.what());
    }
}


void DatabaseManager::insertCharacterEpisode(int characterId, int episodeId) {
    try {
        std::lock_guard<std::mutex> lock(dbMutex);
        pqxx::work txn(*conn);

        pqxx::zview query = R"(
            INSERT INTO character_episodes (character_id, episode_id)
            VALUES ($1, $2)
            ON CONFLICT (character_id, episode_id) DO NOTHING;
        )";
        
        txn.exec(query, pqxx::params{characterId, episodeId});
        txn.commit();

        spdlog::info("Character {} related to Episode {}", characterId, episodeId);
    } catch (const std::exception& e) {
        spdlog::error("Failed to insert character-episode relation: {}", e.what());
    }
}

void DatabaseManager::insertCharacterLocation(int characterId, int locationId) {
    try {
        std::lock_guard<std::mutex> lock(dbMutex);
        pqxx::work txn(*conn);

        // SQL para insertar la relación entre personaje y ubicación
        std::string query = R"(
            INSERT INTO character_locations (character_id, location_id)
            VALUES ($1, $2)
            ON CONFLICT (character_id, location_id) DO NOTHING;
        )";

        txn.exec(query, pqxx::params{characterId, locationId});
        txn.commit();

        spdlog::info("Character {} related to Location {}", characterId, locationId);
    } catch (const std::exception& e) {
        spdlog::error("Failed to insert character-location relation: {}", e.what());
    }
}


DatabaseManager& DatabaseManager::getInstance() {
    try {
        if (!instance) {
            instance = std::make_unique<DatabaseManager>();
        }
        return *instance;
    } catch (const std::exception& e) {
        std::cerr << "Error al crear la instancia de DatabaseManager: " << e.what() << "\n";
        throw;  // Vuelve a lanzar la excepción para que sea manejada arriba
    }
}

void DatabaseManager::updateconnectionStringAndConnect(const std::string& newconnectionString)
{
    std::map<std::string, std::string> connectionParams = parseConnectionString(newconnectionString);
    
    if (connectionParams.find("host") != connectionParams.end()) {
        setHost(connectionParams["host"]);
    }
    if (connectionParams.find("user") != connectionParams.end()) {
        setUser(connectionParams["user"]);
    }
    if (connectionParams.find("port") != connectionParams.end()) {
        setPort(connectionParams["port"]);
    }
    if (connectionParams.find("password") != connectionParams.end()) {
        setPassword(connectionParams["password"]);
    }
    
    // Establecer la conexión a la base de datos
    connectToDatabase();
    

}

void DatabaseManager::connectToDatabase()
{
    std::string connectionString = "host=" + host +
    " user=" + user +
    " port=" + port +
    " password=" + password;

    conn = std::make_unique<pqxx::connection>(connectionString);

    createDataBaseIfNotExist();
    conn->close();
    conn = std::make_unique<pqxx::connection>(connectionString);
 
   createTablesIfNotExist();

   try {
    std::lock_guard<std::mutex> lock(dbMutex);
    pqxx::work txn(*conn);
    
    std::string query = "SELECT table_name FROM information_schema.tables WHERE table_schema = 'public';";
    pqxx::result res = txn.exec(query);

    if (res.empty()) {
        spdlog::info("No tables found in the 'public' schema.");
    } else {
        spdlog::info("Tables in the 'public' schema:");
        for (const auto& row : res) {
            spdlog::info("- {}", row["table_name"].as<std::string>());
        }
    }
    
    txn.commit();
    } catch (const std::exception& e) {
        spdlog::error("Error checking tables: {}", e.what());
    }

    try {
        pqxx::work txn(*conn);
        
        // Consulta para seleccionar el primer valor de la tabla 'characters'
        std::string query = "SELECT * FROM characters LIMIT 1;";
        pqxx::result res = txn.exec(query);
    
        if (res.empty()) {
            spdlog::info("No data found in the 'characters' table.");
        } else {
            spdlog::info("First row in the 'characters' table:");
            
            // Imprime el primer valor de la primera columna de la fila
            spdlog::info("First value: {}", res[0][0].c_str());  // Asumiendo que la primera columna es un texto
        }
        
        txn.commit();
    } catch (const std::exception& e) {
        spdlog::error("Error retrieving data from 'characters' table: {}", e.what());
    }

    ApiLoader::getInstance().start();

}


std::map<std::string, std::string> DatabaseManager::parseConnectionString(const std::string& connectionString) {
    std::map<std::string, std::string> params;
    std::stringstream ss(connectionString);
    std::string token;
    
    while (std::getline(ss, token, ' ')) {
        size_t equalPos = token.find('=');
        if (equalPos != std::string::npos) {
            std::string key = token.substr(0, equalPos);
            std::string value = token.substr(equalPos + 1);
            params[key] = value;
        }
    }
    
    return params;
}

bool DatabaseManager::isConnected() {

    return conn && conn->is_open();
}

void DatabaseManager::createDataBaseIfNotExist()
{
    try {
        std::lock_guard<std::mutex> lock(dbMutex);
        // Crear la base de datos si no existe
        std::string createDatabaseQuery = "CREATE DATABASE " + database + ";";
        pqxx::nontransaction txn(*conn);
        txn.exec(createDatabaseQuery);
        txn.commit();

        spdlog::info("Database '{}' created successfully.", database);

    } catch (const pqxx::sql_error& e) {
        if (std::string(e.what()).find("exist") == std::string::npos) {
            spdlog::error("Failed to create database: {}", e.what());
            throw;
        } else {
            spdlog::info("Database '{}' already exists.", database);
        }
    } catch (const std::exception& e) {
        spdlog::error("Error while connecting to server or creating database: {}", e.what());
        throw;
    }
    
}

void DatabaseManager::createTablesIfNotExist() {
    try {
        std::lock_guard<std::mutex> lock(dbMutex);
        pqxx::work txn(*conn);

        // SQL para crear la tabla de personajes
        std::string createCharactersTableQuery = R"(
            CREATE TABLE IF NOT EXISTS characters (
                id SERIAL PRIMARY KEY,
                name TEXT NOT NULL,
                status TEXT,
                species TEXT,
                type TEXT,
                gender TEXT,
                origin_name TEXT,
                origin_url TEXT,
                location_name TEXT,
                location_url TEXT,
                image_url TEXT,
                url TEXT,
                created TIMESTAMP
            );
        )";

        // SQL para crear la tabla de ubicaciones
        std::string createLocationsTableQuery = R"(
            CREATE TABLE IF NOT EXISTS locations (
                id SERIAL PRIMARY KEY,
                name TEXT NOT NULL,
                type TEXT,
                dimension TEXT,
                url TEXT,
                created TIMESTAMP
            );
        )";

        // SQL para crear la tabla de episodios
        std::string createEpisodesTableQuery = R"(
            CREATE TABLE IF NOT EXISTS episodes (
                id SERIAL PRIMARY KEY,
                name TEXT NOT NULL,
                air_date TIMESTAMP,
                episode TEXT,
                url TEXT,
                created TIMESTAMP
            );
        )";

        // SQL para crear la tabla de relaciones de personajes y ubicaciones
        std::string createCharacterLocationsTableQuery = R"(
            CREATE TABLE IF NOT EXISTS character_locations (
                character_id INT NOT NULL,
                location_id INT NOT NULL,
                PRIMARY KEY (character_id, location_id),
                FOREIGN KEY (character_id) REFERENCES characters(id) ON DELETE CASCADE,
                FOREIGN KEY (location_id) REFERENCES locations(id) ON DELETE CASCADE
            );
        )";


        // SQL para crear la tabla de relaciones de personajes y episodios
        std::string createCharacterEpisodesTableQuery = R"(
            CREATE TABLE IF NOT EXISTS character_episodes (
                character_id INT NOT NULL,
                episode_id INT NOT NULL,
                PRIMARY KEY (character_id, episode_id),
                FOREIGN KEY (character_id) REFERENCES characters(id) ON DELETE CASCADE,
                FOREIGN KEY (episode_id) REFERENCES episodes(id) ON DELETE CASCADE
            );
        )";



        // Ejecutar las consultas para crear las tablas
        txn.exec(createLocationsTableQuery);
        txn.exec(createEpisodesTableQuery);
        txn.exec(createCharactersTableQuery);
        txn.exec(createCharacterLocationsTableQuery);
        txn.exec(createCharacterEpisodesTableQuery);




        txn.commit();

        spdlog::info("Tables created if they did not exist.");
    } catch (const std::exception& e) {
        spdlog::error("Error creating tables: {}", e.what());
    }
}
