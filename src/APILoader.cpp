#include "ApiLoader.h"
#include <iostream>
#include <chrono>
#include "DatabaseManager.h"
#include "ApiClient.h"
#include "BatchProcessor.h"

ApiLoader& ApiLoader::getInstance(int intervalSeconds) {
    static ApiLoader instance(intervalSeconds);
    return instance;
}

ApiLoader::ApiLoader(int intervalSeconds) : interval(intervalSeconds), stop(false), running(false) {}

void ApiLoader::start() {
    std::lock_guard<std::mutex> lock(mutex);
    if (!running) {
        stop = false;
        running = true;
        workerThread = std::thread(&ApiLoader::run, this);
    }
}

void ApiLoader::stopLoading() {
    {
        std::lock_guard<std::mutex> lock(mutex);
        stop = true;
    }
    if (workerThread.joinable()) {
        workerThread.join();
    }
    running = false;
}

void ApiLoader::loadDataManually() {
    std::lock_guard<std::mutex> lock(mutex);
    if (!loadDataFromApi()) {
        std::cerr << "Error: No se pudo cargar los datos manualmente debido a problemas de conexión.\n";
    }
}

void ApiLoader::updateInterval(const int& newIntervalSeconds) {
    std::lock_guard<std::mutex> lock(mutex);
    interval = newIntervalSeconds;
}

int ApiLoader::getInterval() const {
    return interval;
}

ApiLoader::~ApiLoader() {
    stopLoading();
}

void ApiLoader::run() {
    while (true) {
        {
            std::lock_guard<std::mutex> lock(mutex);
            if (stop) break;
        }

        if (!loadDataFromApi()) {
            std::cerr << "Error: No se pudo cargar los datos automáticamente debido a problemas de conexión.\n";
            break; // Detiene el bucle si la conexión falla
        }

        std::this_thread::sleep_for(std::chrono::seconds(interval));
    
    }
    running = false; // Asegúrate de que running se actualice cuando el hilo termine
}

bool ApiLoader::loadDataFromApi() {
    //std::cout << "Loading data from API...\n";
    DatabaseManager& dbManager = DatabaseManager::getInstance();
    if (!dbManager.isConnected()) {
        std::cerr << "Error: La base de datos no está conectada.\n";
        return false;
    }
    try {
        BatchProcessor batchProcessor(ApiClient::getInstance(), DatabaseManager::getInstance());
        batchProcessor.execute();
        //std::cout << "Data loaded successfully.\n";
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error inesperado al cargar los datos: " << e.what() << std::endl;
        return false;
    }
}