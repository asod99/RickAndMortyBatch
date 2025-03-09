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
        src/BatchProcessor.cpp
        src/LoggerMenu.cpp
        src/Logger.cpp
        src/Config.cpp
        src/Menu.cpp
    )

    # Include directories
    target_include_directories(RickAndMortyBatch PRIVATE include)

    # Add compiler option for UTF-8
    if(MSVC)
        target_compile_options(RickAndMortyBatch PRIVATE /utf-8)
    endif()

    # Use find_package to locate the libraries
    find_package(CURL REQUIRED)
    find_package(JsonCpp CONFIG REQUIRED)
    find_package(libpqxx REQUIRED)
    find_package(spdlog CONFIG REQUIRED)

    # Link the libraries
    target_link_libraries(RickAndMortyBatch PRIVATE CURL::libcurl JsonCpp::JsonCpp libpqxx::pqxx spdlog::spdlog)


    # Set output directories
    set_target_properties(RickAndMortyBatch PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    )

    # Copy config file to build directory
    configure_file(config/config.json ${CMAKE_BINARY_DIR}/bin/config.json COPYONLY)