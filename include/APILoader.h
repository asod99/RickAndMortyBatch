#ifndef APILOADER_H
#define APILOADER_H

#include <thread>
#include <mutex>

#define UPDATE_TIME_SECONDS 86400 ///< Default update interval in seconds (24 hours).

/**
 * @class ApiLoader
 * @brief Class to manage periodic data loading from an API.
 */
class ApiLoader {
public:
    /**
     * @brief Gets the singleton instance of ApiLoader.
     * @param intervalSeconds The interval in seconds for data updates.
     * @return The unique instance of ApiLoader.
     */
    static ApiLoader& getInstance(int intervalSeconds = UPDATE_TIME_SECONDS);

    /**
     * @brief Starts the data loading process in a separate thread.
     */
    void start();

    /**
     * @brief Stops the data loading process.
     */
    void stopLoading();

    /**
     * @brief Manually triggers data loading from the API.
     */
    void loadDataManually();

    /**
     * @brief Updates the interval for data loading.
     * @param newIntervalSeconds The new interval in seconds.
     */
    void updateInterval(const int& newIntervalSeconds);

    /**
     * @brief Gets the current interval for data loading.
     * @return The interval in seconds.
     */
    int getInterval() const;

    // Delete copy constructor and assignment operator
    ApiLoader(const ApiLoader&) = delete;
    ApiLoader& operator=(const ApiLoader&) = delete;

private:
    /**
     * @brief Constructs an ApiLoader with a specified interval.
     * @param intervalSeconds The interval in seconds for data updates.
     */
    ApiLoader(int intervalSeconds);  // Private constructor

    /**
     * @brief Destructor for ApiLoader.
     */
    ~ApiLoader();

    /**
     * @brief The main loop that runs in a separate thread to load data periodically.
     */
    void run();

    /**
     * @brief Loads data from the API.
     * @return True if data was successfully loaded, false otherwise.
     */
    bool loadDataFromApi();

    int interval; ///< Interval in seconds for data updates.
    bool stop; ///< Flag to stop the data loading process.
    bool running; ///< Indicates if the thread is currently running.
    std::thread workerThread; ///< Thread for running the data loading process.
    std::mutex mutex; ///< Mutex for synchronizing access to shared resources.
};

#endif // APILOADER_H