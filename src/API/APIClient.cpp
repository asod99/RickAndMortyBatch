#include "ApiClient.h"
#include <curl/curl.h>
#include <spdlog/spdlog.h>
#include <sstream>
#include <stdexcept>
#include <utility>
#include <iostream>

std::unique_ptr<ApiClient> ApiClient::_instance = nullptr;

ApiClient::ApiClient() {
    spdlog::info("ApiClient initialized.");
}

Json::Value ApiClient::getResource(std::string_view resource, int page, const std::unordered_map<std::string, std::string>& filters) {
    std::string url = constructUrl(resource, page, filters);
    spdlog::info("Fetching resource from API: {}", url);

    std::string response = httpGet(url);

    Json::CharReaderBuilder readerBuilder;
    Json::Value root;
    std::string errs;
    std::istringstream s(response);
    if (!Json::parseFromStream(readerBuilder, s, &root, &errs)) {
        spdlog::error("Failed to parse JSON: {}", errs);
        throw std::runtime_error("Failed to parse JSON: " + errs);
    }

    spdlog::info("Successfully fetched and parsed resource from API.");
    return root;
}

std::string ApiClient::constructUrl(std::string_view resource, int page, const std::unordered_map<std::string, std::string>& filters) {
    std::ostringstream url;
    url << _baseUrl << "/" << resource << "?page=" << page;

    for (const auto& [key, value] : filters) {
        url << "&" << key << "=" << value;
    }

    spdlog::debug("Constructed URL: {}", url.str());
    return url.str();
}

std::string ApiClient::httpGet(std::string_view url) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        spdlog::error("Failed to initialize CURL.");
        throw std::runtime_error("Failed to initialize CURL.");
    }

    std::string response;
    curl_easy_setopt(curl, CURLOPT_URL, url.data());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    spdlog::info("Performing HTTP GET request to URL: {}", url);
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        spdlog::error("CURL request error: {}", curl_easy_strerror(res));
        curl_easy_cleanup(curl);
        throw std::runtime_error("CURL request error: " + std::string(curl_easy_strerror(res)));
    }

    spdlog::info("HTTP GET request successful.");
    curl_easy_cleanup(curl);
    return response;
}

size_t ApiClient::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

ApiClient& ApiClient::getInstance() {
    if (!_instance) {
        spdlog::info("Creating a new instance of ApiClient.");
        _instance = std::unique_ptr<ApiClient>(new ApiClient());
    } else {
        spdlog::info("Returning existing instance of ApiClient.");
    }
    return *_instance;
}

std::string ApiClient::getBaseUrl() const {
    spdlog::debug("Getting base URL: {}", _baseUrl);
    return _baseUrl;
}

void ApiClient::updateBaseUrl(const std::string& newBaseUrl) {
    spdlog::info("Updating API base URL to: {}", newBaseUrl);
    _baseUrl = newBaseUrl;
}