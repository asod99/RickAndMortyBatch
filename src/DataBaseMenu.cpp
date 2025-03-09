#include <string>
#include <pqxx/pqxx>
#include <DataBaseMenu.h>

using namespace std;

DatabaseMenu::DatabaseMenu(std::unique_ptr<pqxx::connection> conn) : conn(std::move(conn)){}

void DatabaseMenu::showHelp() {
    //Hay que poner el help
    cout << "Available commands (order does not matter):" << endl;
    cout << "  characters --filter <field>={value} --only <field> --size --size-only" << endl;
    cout << "  episodes --filter <field>={value} --only <field> --size --size-only" << endl;
    cout << "  locations --filter <field>={value} --only <field> --size --size-only" << endl;
    cout << "Options:" << endl;
    cout << "  --filter <field>={value} : Filter by field, use '{value}' for starts with" << endl;
    cout << "  --only <field> : Display only specified field" << endl;
    cout << "  --size : Show the number of results" << endl;
    cout << "  --size-only : Show only the number of results" << endl;
    cout << "Commands can be entered in any order." << endl;
}

void DatabaseMenu::executeQuery(const string& query, vector<string> onlyFields, bool showSize, bool sizeOnly) {
    pqxx::work txn(*conn);
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
    vector<string> validTables = {"characters", "episodes", "locations"};
    istringstream iss(command);
    vector<string> args{istream_iterator<string>{iss}, istream_iterator<string>{}};
    
    if (args.empty() || 
    std::find(args.begin(), args.end(), "-h") != args.end() || 
    std::find(args.begin(), args.end(), "--help") != args.end()) {
    showHelp();
    return;
}
    
    string table = args[0];
    if (find(validTables.begin(), validTables.end(), table) == validTables.end()) {
        cout << "Invalid table. Available options: characters, episodes, locations" << endl;
        return;
    }
    
    string query = "SELECT * FROM " + table;
    vector<string> filters;
    vector<string> onlyFields;
    bool showSize = false, sizeOnly = false;
    
    for (size_t i = 1; i < args.size(); ++i) {
        if (args[i] == "--filter" && i + 1 < args.size()) {
            while (i + 1 < args.size() && args[i + 1].find("=") != string::npos) {
                filters.push_back(args[++i]);
            }
        } else if (args[i] == "--only" && i + 1 < args.size()) {
            onlyFields.push_back(args[++i]);
        } else if (args[i] == "--size") {
            showSize = true;
        } else if (args[i] == "--size-only") {
            sizeOnly = true;
        }
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
    
    executeQuery(query, onlyFields, showSize, sizeOnly);
}

void DatabaseMenu::dataBaseMenu() {
    string command;
    cout << "--MENU SEARCH DATABASE--" << std::endl;
    while (true) {
        cout << "Enter command: ";
        getline(cin, command);
        
        if (command == "exit") break;
        
        if(conn && conn->is_open())
        {
            processCommand(command);
        }
        else
        {
            cout << "Database is not connected";
            break;
        }
    }
}



