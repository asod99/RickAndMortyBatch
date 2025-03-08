#include "DatabaseManager.h"
#include <spdlog/spdlog.h>

DatabaseManager::DatabaseManager(const std::string& connectionString)
    : conn(connectionString) {
    spdlog::info("DatabaseManager initialized and connected to database.");
}

void DatabaseManager::insertCharacter(int id, const std::string& name, const std::string& status,
                                      const std::string& species, const std::string& type, const std::string& gender,
                                      const std::string& originName, const std::string& originUrl,
                                      const std::string& locationName, const std::string& locationUrl,
                                      const std::string& image, const std::string& url, const std::string& created) {
    try {
        pqxx::work txn(conn);
        txn.exec_params(R"(
            INSERT INTO characters (id, name, status, species, type, gender, origin_name, origin_url, location_name, location_url, image, url, created)
            VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13)
            ON CONFLICT (id) DO NOTHING;)",
            id, name, status, species, type, gender, originName, originUrl, locationName, locationUrl, image, url, created);
        txn.commit();
        spdlog::info("Character inserted into database: {}", name);
    } catch (const std::exception& e) {
        spdlog::error("Failed to insert character: {}", e.what());
    }
}

void DatabaseManager::insertLocation(int id, const std::string& name, const std::string& type,
                                     const std::string& dimension, const std::string& url, const std::string& created) {
    try {
        pqxx::work txn(conn);
        txn.exec_params(R"(
            INSERT INTO locations (id, name, type, dimension, url, created)
            VALUES ($1, $2, $3, $4, $5, $6)
            ON CONFLICT (id) DO NOTHING;)",
            id, name, type, dimension, url, created);
        txn.commit();
        spdlog::info("Location inserted into database: {}", name);
    } catch (const std::exception& e) {
        spdlog::error("Failed to insert location: {}", e.what());
    }
}

void DatabaseManager::insertEpisode(int id, const std::string& name, const std::string& airDate,
                                    const std::string& episode, const std::string& url, const std::string& created) {
    try {
        pqxx::work txn(conn);
        txn.exec_params(R"(
            INSERT INTO episodes (id, name, air_date, episode, url, created)
            VALUES ($1, $2, $3, $4, $5, $6)
            ON CONFLICT (id) DO NOTHING;)",
            id, name, airDate, episode, url, created);
        txn.commit();
        spdlog::info("Episode inserted into database: {}", name);
    } catch (const std::exception& e) {
        spdlog::error("Failed to insert episode: {}", e.what());
    }
}