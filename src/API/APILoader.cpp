#include "ApiLoader.h"
#include <iostream>
#include <chrono>
#include "DatabaseManager.h"
#include "ApiClient.h"
#include "BatchProcessor.h"
#include <spdlog/spdlog.h>

ApiLoader& ApiLoader::getInstance(int intervalSeconds) {
    static ApiLoader instance(intervalSeconds);
    spdlog::info("ApiLoader instance created with interval: {} seconds", intervalSeconds);
    return instance;
}

ApiLoader::ApiLoader(int intervalSeconds) : _interval(intervalSeconds), _stop(false), _running(false) {
    spdlog::info("ApiLoader initialized with interval: {} seconds", intervalSeconds);
}

void ApiLoader::start() {
    std::lock_guard<std::mutex> lock(_mutex);
    if (!_running) {
        _stop = false;
        _running = true;
        spdlog::info("Starting ApiLoader thread.");
        _workerThread = std::thread(&ApiLoader::run, this);
    } else {
        spdlog::warn("ApiLoader thread is already running.");
    }
}

void ApiLoader::stopLoading() {
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _stop = true;
    }
    if (_workerThread.joinable()) {
        _workerThread.join();
        spdlog::info("ApiLoader thread stopped.");
    }
    _running = false;
}

void ApiLoader::loadDataManually() {
    std::lock_guard<std::mutex> lock(_mutex);
    spdlog::info("Manual data load triggered.");
    if (!loadDataFromApi()) {
        spdlog::error("Failed to manually load data due to connection issues.");
    } else {
        spdlog::info("Data loaded manually successfully.");
    }
}

void ApiLoader::updateInterval(const int& newIntervalSeconds) {
    std::lock_guard<std::mutex> lock(_mutex);
    spdlog::info("Updating interval from {} to {} seconds", _interval, newIntervalSeconds);
    _interval = newIntervalSeconds;
}

int ApiLoader::getInterval() const {
    spdlog::debug("Retrieving current interval: {} seconds", _interval);
    return _interval;
}

ApiLoader::~ApiLoader() {
    spdlog::info("Destroying ApiLoader and stopping thread if running.");
    stopLoading();
}

void ApiLoader::run() {
    spdlog::info("ApiLoader thread running.");
    while (true) {
        {
            std::lock_guard<std::mutex> lock(_mutex);
            if (_stop) {
                spdlog::info("Stop signal received, breaking the loop.");
                break;
            }
        }

        if (!loadDataFromApi()) {
            spdlog::error("Failed to automatically load data due to connection issues.");
            break;
        }

        spdlog::info("Sleeping for {} seconds before next data load.", _interval);
        std::this_thread::sleep_for(std::chrono::seconds(_interval));
    }
    _running = false;
    spdlog::info("ApiLoader thread finished.");
}

bool ApiLoader::loadDataFromApi() {
    spdlog::info("Loading data from API...");
    DatabaseManager& dbManager = DatabaseManager::getInstance();
    if (!dbManager.isConnected()) {
        spdlog::error("Database is not connected.");
        return false;
    }
    try {
        BatchProcessor batchProcessor(ApiClient::getInstance(), DatabaseManager::getInstance());
        batchProcessor.execute();
        spdlog::info("Data loaded successfully.");
        return true;
    } catch (const std::exception& e) {
        spdlog::error("Unexpected error while loading data: {}", e.what());
        return false;
    }
}