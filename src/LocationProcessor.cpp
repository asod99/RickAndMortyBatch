#include "LocationProcessor.h"
#include <spdlog/spdlog.h>

LocationProcessor::LocationProcessor(DatabaseManager& dbManager)
    : dbManager(dbManager) {
    spdlog::info("LocationProcessor initialized.");
}

void LocationProcessor::process(const Json::Value& data) {
    spdlog::info("Processing location data.");

    for (const auto& location : data) {
        int id = location["id"].asInt();
        std::string name = location["name"].asString();
        std::string type = location["type"].asString();
        std::string dimension = location["dimension"].asString();
        std::string url = location["url"].asString();
        std::string created = location["created"].asString();

        // Insertar la ubicación en la base de datos
        dbManager.insertLocation(id, name, type, dimension,url, created);

        // Ahora procesamos la lista de personajes que han estado en esta ubicación
        const Json::Value& characters = location["residents"];  // "residents" es la lista de personajes

        for (const auto& characterUrl : characters) {
            std::string characterUrlString = characterUrl.asString();
            int characterId = extractCharacterIdFromUrl(characterUrlString);
            if (characterId != -1) {
                dbManager.insertCharacterLocation(characterId, id); // Relacionamos el personaje con la ubicación
            }
        }
    }
}

// Función auxiliar para extraer el ID del personaje desde la URL
int LocationProcessor::extractCharacterIdFromUrl(const std::string& characterUrl) {
    // Asumiendo que la URL es como "https://rickandmortyapi.com/api/character/1", extraemos el ID
    size_t lastSlashPos = characterUrl.find_last_of('/');
    if (lastSlashPos != std::string::npos) {
        return std::stoi(characterUrl.substr(lastSlashPos + 1)); // Extrae el número después de la última barra
    }
    return -1; // Retorna -1 si la URL no es válida
}