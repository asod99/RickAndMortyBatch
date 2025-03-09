#include <string>
#include <pqxx/pqxx>
#include <DataBaseMenu.h>
#include <ValidFields.h>
#include "DataBaseManager.h"
#include <mutex>
#include <spdlog/spdlog.h>

using namespace std;

void DatabaseMenu::showHelp() {
    cout << "Available commands (order does not matter):" << endl;
    cout << "  characters --filter <field>={value} --only <field> --size --size-only" << endl;
    cout << "  episodes --filter <field>={value} --only <field> --size --size-only" << endl;
    cout << "  locations --filter <field>={value} --only <field> --size --size-only" << endl;
    cout << "Options:" << endl;
    cout << "  --filter <field>={value} : Filter by field, use '{value}' for starts with" << endl;
    cout << "  --only <field> : Display only specified field" << endl;
    cout << "  --size : Show the number of results" << endl;
    cout << "  --size-only : Show only the number of results" << endl;
}

string join(const vector<string>& elements, const string& delimiter) {
    ostringstream oss;
    for (size_t i = 0; i < elements.size(); ++i) {
        if (i > 0) oss << delimiter;
        oss << elements[i];
    }
    return oss.str();
}

void DatabaseMenu::executeQuery(const string& query, vector<string> onlyFields, bool showSize, bool sizeOnly) {
    DatabaseManager& dbManager = DatabaseManager::getInstance();
    std::lock_guard<std::mutex> lock(dbManager.dbMutex);

    if (!dbManager.isConnected()) {
        spdlog::error("Database is not connected.");
        return;
    }

    spdlog::info("Executing query: {}", query);
    pqxx::work txn(dbManager.getConn());
    pqxx::result res = txn.exec(query);

    if (showSize || sizeOnly) {
        cout << "Total results: " << res.size() << endl;
        if (sizeOnly) return;
    }

    for (const auto& row : res) {
        for (const auto& field : row) {
            if (onlyFields.empty() || find(onlyFields.begin(), onlyFields.end(), field.name()) != onlyFields.end()) {
                cout << field.name() << ": " << field.c_str() << " | ";
            }
        }
        cout << endl;
    }
}

void DatabaseMenu::processCommand(const string& command) {
    spdlog::info("Processing command: {}", command);
    vector<string> validTables = {"characters", "episodes", "locations"};
    istringstream iss(command);
    vector<string> args{istream_iterator<string>{iss}, istream_iterator<string>{}};

    if (args.empty() || 
        std::find(args.begin(), args.end(), "-h") != args.end() || 
        std::find(args.begin(), args.end(), "--help") != args.end()) {
        showHelp();
        return;
    }

    if (args[0] == "all") {
        for (const auto& table : validTables) {
            cout << "Results from table: " << table << endl;
            spdlog::info("Executing query for table: {}", table);
            executeQueryForTable(table, args);
        }
        return;
    }

    string table = args[0];
    if (find(validTables.begin(), validTables.end(), table) == validTables.end()) {
        cout << "Invalid table. Available options: characters, episodes, locations" << endl;
        return;
    }

    executeQueryForTable(table, args);
}

void DatabaseMenu::executeQueryForTable(const string& table, const vector<string>& args) {
    string query = "SELECT * FROM " + table;
    vector<string> filters;
    vector<string> onlyFields;
    bool showSize = false, sizeOnly = false;

    for (size_t i = 1; i < args.size(); ++i) {
        if (args[i] == "--filter" && i + 1 < args.size()) {
            while (i + 1 < args.size() && args[i + 1].find("=") != string::npos) {
                std::string filter = args[++i];
                size_t equalPos = filter.find("=");
                if (equalPos != string::npos) {
                    std::string field = filter.substr(0, equalPos);
                    if (validateField(table, field)) {
                        filters.push_back(filter);
                    } else {
                        std::cout << "Invalid field: " << field << " for table " << table << std::endl;
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
            string field = filters[i].substr(0, filters[i].find("="));
            string value = filters[i].substr(filters[i].find("=") + 1);
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
    string command;
    cout << "--MENU SEARCH DATABASE--" << std::endl;
    while (true) {
        cout << "Enter command: ";
        getline(cin, command);

        if (command == "exit") {
            break;
        }

        processCommand(command);
    }
}