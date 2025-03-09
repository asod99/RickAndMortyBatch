#ifndef VALID_FIELDS_H
#define VALID_FIELDS_H

#include <string>
#include <unordered_set>
#include <unordered_map>

/**
 * @brief Maps table names to sets of valid fields.
 *
 * This constant map is used to define the valid fields for each database table.
 * It ensures that operations involving field validation can quickly check if a
 * field is valid for a given table. This is particularly useful for query
 * validation and dynamic query generation.
 *
 * The map contains the following entries:
 * - "characters": Fields related to character information.
 * - "locations": Fields related to location information.
 * - "episodes": Fields related to episode information.
 */
const std::unordered_map<std::string, std::unordered_set<std::string>> validFields = {
    {"characters", {
        "id", "name", "status", "species", "type", "gender", 
        "origin_name", "origin_url", "location_name", "location_url", 
        "image_url", "url", "created"
    }},
    {"locations", {
        "id", "name", "type", "dimension", "url", "created"
    }},
    {"episodes", {
        "id", "name", "air_date", "episode", "url", "created"
    }}
};

#endif // VALID_FIELDS_H