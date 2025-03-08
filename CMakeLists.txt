cmake_minimum_required(VERSION 3.10)

# Project name and version
project(RickAndMortyBatch VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add executable
add_executable(RickAndMortyBatch
    src/main.cpp
    src/ApiClient.cpp
    src/CharacterProcessor.cpp
    src/LocationProcessor.cpp
    src/EpisodeProcessor.cpp
    src/DatabaseManager.cpp
    src/Logger.cpp
    src/Config.cpp
)

# Include directories
target_include_directories(RickAndMortyBatch PRIVATE include)

# Find and link external libraries
find_package(spdlog REQUIRED)
find_package(jsoncpp REQUIRED)
find_package(PostgreSQL REQUIRED)

target_link_libraries(RickAndMortyBatch PRIVATE spdlog::spdlog jsoncpp_lib pqxx)

# Set output directories
set_target_properties(RickAndMortyBatch PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Copy config file to build directory
configure_file(config/config.json ${CMAKE_BINARY_DIR}/bin/config.json COPYONLY)