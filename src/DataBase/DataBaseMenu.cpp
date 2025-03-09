#include <string>
#include <pqxx/pqxx>
#include <DataBaseMenu.h>
#include <ValidFields.h>
#include "DataBaseManager.h"
#include <mutex>
#include <spdlog/spdlog.h>
#include "Colors.h"

void DatabaseMenu::showHelp() {
    Color::printAnimatedText("Available commands (order does not matter):", Color::cyan);
    std::cout << Color::yellow << "  characters --filter <field>={value} --only <field> --size --size-only" << Color::reset << std::endl;
    std::cout << Color::yellow << "  episodes   --filter <field>={value} --only <field> --size --size-only" << Color::reset << std::endl;
    std::cout << Color::yellow << "  locations  --filter <field>={value} --only <field> --size --size-only" << Color::reset << std::endl;
    std::cout << Color::yellow << "  all        --filter <field>={value} --only <field> --size --size-only" << Color::reset << std::endl;
    std::cout << Color::green << "Options:" << Color::reset << std::endl;
    std::cout << Color::cyan << "  --filter <field>={value} : Filter by field, use '{value}' for starts with" << Color::reset << std::endl;
    std::cout << Color::cyan << "  --only <field> : Display only specified field" << Color::reset << std::endl;
    std::cout << Color::cyan << "  --size : Show the number of results" << Color::reset << std::endl;
    std::cout << Color::cyan << "  --size-only : Show only the number of results" << Color::reset << std::endl;
}

std::string join(const std::vector<std::string>& elements, const std::string& delimiter) {
    std::ostringstream oss;
    for (size_t i = 0; i < elements.size(); ++i) {
        if (i > 0) oss << delimiter;
        oss << elements[i];
    }
    return oss.str();
}

void DatabaseMenu::executeQuery(const std::string& query, std::vector<std::string> onlyFields, bool showSize, bool sizeOnly) {
    DatabaseManager& dbManager = DatabaseManager::getInstance();
    std::lock_guard<std::mutex> lock(dbManager._dbMutex);

    if (!dbManager.isConnected()) {
        spdlog::error("Database is not connected.");
        return;
    }

    spdlog::info("Executing query: {}", query);
    pqxx::work txn(dbManager.getConn());
    pqxx::result res = txn.exec(query);

    if (showSize || sizeOnly) {
        std::cout << Color::magenta << "Total results: " << res.size() << Color::reset << std::endl;
        if (sizeOnly) return;
    }

    for (const auto& row : res) {
        for (const auto& field : row) {
            if (onlyFields.empty() || find(onlyFields.begin(), onlyFields.end(), field.name()) != onlyFields.end()) {
                std::cout << Color::cyan << field.name() << Color::reset << ": " << Color::green << field.c_str() << Color::reset << " | ";
            }
        }
        std::cout << std::endl;
    }
}

void DatabaseMenu::processCommand(const std::string& command) {
    spdlog::info("Processing command: {}", command);
    std::vector<std::string> validTables = {"characters", "episodes", "locations"};
    std::istringstream iss(command);
    std::vector<std::string> args{std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{}};

    if (args.empty() || 
        std::find(args.begin(), args.end(), "-h") != args.end() || 
        std::find(args.begin(), args.end(), "--help") != args.end()) {
        showHelp();
        return;
    }

    if (args[0] == "all") {
        for (const auto& table : validTables) {
            Color::printAnimatedText("Results from table: " + table, Color::yellow);
            spdlog::info("Executing query for table: {}", table);
            executeQueryForTable(table, args);
        }
        return;
    }

    std::string table = args[0];
    if (find(validTables.begin(), validTables.end(), table) == validTables.end()) {
        std::cout << Color::red << "Invalid table. Available options: characters, episodes, locations" << Color::reset << std::endl;
        return;
    }

    executeQueryForTable(table, args);
}

void DatabaseMenu::executeQueryForTable(const std::string& table, const std::vector<std::string>& args) {
    std::string query = "SELECT * FROM " + table;
    std::vector<std::string> filters;
    std::vector<std::string> onlyFields;
    bool showSize = false, sizeOnly = false;

    for (size_t i = 1; i < args.size(); ++i) {
        if (args[i] == "--filter" && i + 1 < args.size()) {
            while (i + 1 < args.size() && args[i + 1].find("=") != std::string::npos) {
                std::string filter = args[++i];
                size_t equalPos = filter.find("=");
                if (equalPos != std::string::npos) {
                    std::string field = filter.substr(0, equalPos);
                    if (validateField(table, field)) {
                        filters.push_back(filter);
                    } else {
                        std::cout << Color::red << "Invalid field: " << field << " for table " << table << Color::reset << std::endl;
                    }
                }
            }
        } else if (args[i] == "--only" && i + 1 < args.size()) {
            while (i + 1 < args.size() && args[i + 1][0] != '-') {
                std::string field = args[++i];
                if(validateField(table, field))
                    onlyFields.push_back(field);
            }
        } else if (args[i] == "--size") {
            showSize = true;
        } else if (args[i] == "--size-only") {
            sizeOnly = true;
        }
    }

    if (!onlyFields.empty()) {
        query = "SELECT " + join(onlyFields, ", ") + " FROM " + table;
    }

    if (!filters.empty()) {
        query += " WHERE ";
        for (size_t i = 0; i < filters.size(); ++i) {
            if (i > 0) query += " AND ";
            std::string field = filters[i].substr(0, filters[i].find("="));
            std::string value = filters[i].substr(filters[i].find("=") + 1);
            if (value[0] == '{') {
                value = value.substr(1, value.size() - 2) + "%";
                query += field + " LIKE '" + value + "'";
            } else {
                query += field + " = '" + value + "'";
            }
        }
    }

    spdlog::info("Final query for table {}: {}", table, query);
    executeQuery(query, onlyFields, showSize, sizeOnly);
}

bool DatabaseMenu::validateField(const std::string& tableName, const std::string& field) {
    auto it = validFields.find(tableName);
    if (it == validFields.end()) {
        return false;
    }
    const auto& validFieldSet = it->second;
    return validFieldSet.find(field) != validFieldSet.end();
}

void DatabaseMenu::dataBaseMenu() {
    std::string command;
    Color::printAnimatedText("--MENU SEARCH DATABASE--", Color::magenta);
    while (true) {
        std::cout << Color::cyan << "Enter command: " << Color::reset;
        getline(std::cin, command);

        if (command == "exit") {
            break;
        }

        processCommand(command);
    }
}