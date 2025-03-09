#ifndef DATABASEMENU_H
#define DATABASEMENU_H

#include <iostream>
#include <pqxx/pqxx>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>


class DatabaseMenu {
public:
DatabaseMenu(std::unique_ptr<pqxx::connection> connStr);

    void showHelp();

    void executeQuery(const std::string& query, std::vector<std::string> onlyFields, bool showSize, bool sizeOnly);

    void processCommand(const std::string& command);

    void dataBaseMenu();

private:
    std::unique_ptr<pqxx::connection> conn;

};


#endif // DATABASEMENU_H