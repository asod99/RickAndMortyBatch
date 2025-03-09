#ifndef APICLIENT_H
#define APICLIENT_H

#include <string>
#include <string_view>
#include <optional>
#include <json/json.h>
#include <unordered_map>

/**
 * @class ApiClient
 * @brief Class to handle interactions with the Rick and Morty API.
 */
class ApiClient {
public:

    // Evitar copia y asignación
    ApiClient(const ApiClient&) = delete;
    ApiClient& operator=(const ApiClient&) = delete;

    // Método estático para obtener la instancia única
    static ApiClient& getInstance();

    /**
     * @brief Constructs an ApiClient.
     * @param baseUrl The base URL of the API.
     */
    explicit ApiClient();

    /**
     * @brief Retrieves data from a specific API resource with optional filters.
     * @param resource The resource to fetch (e.g., "character", "location", "episode").
     * @param page The page number to fetch.
     * @param filters Optional filters to apply to the API request.
     * @return JSON data from the API response.
     * @throws std::runtime_error if an error occurs during the request.
     */
    Json::Value getResource(std::string_view resource, int page = 1, const std::unordered_map<std::string, std::string>& filters = {});

    void updateBaseUrl(const std::string& newBaseUrl);

    std::string getBaseUrl() const;

private:
    static std::unique_ptr<ApiClient> instance;
    std::string baseUrl; ///< Base URL of the API.

    /**
     * @brief Constructs a URL with query parameters.
     * @param resource The resource to fetch.
     * @param page The page number to fetch.
     * @param filters Filters to apply to the API request.
     * @return A complete URL with query parameters.
     */
    std::string constructUrl(std::string_view resource, int page, const std::unordered_map<std::string, std::string>& filters);

    /**
     * @brief Performs an HTTP GET request to a specific URL.
     * @param url The URL to send the request to.
     * @return The response from the request as a string.
     * @throws std::runtime_error if an error occurs during the request.
     */
    std::string httpGet(std::string_view url);

    /**
     * @brief Callback to write response data.
     * @param contents Data received.
     * @param size Size of each data block.
     * @param nmemb Number of data blocks.
     * @param userp Pointer to user data.
     * @return Number of bytes processed.
     */
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);
};

#endif // APICLIENT_H