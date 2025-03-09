#ifndef DATABASEMENU_H
#define DATABASEMENU_H

#include <iostream>
#include <pqxx/pqxx>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>

/**
 * @class DatabaseMenu
 * @brief Class to manage user interaction with the database through a menu interface.
 *
 * This class provides methods to display help, execute queries, process commands,
 * and validate fields for database operations.
 */
class DatabaseMenu {
public:
    /**
     * @brief Displays help information for the database menu.
     */
    void showHelp();

    /**
     * @brief Executes a SQL query and displays the results.
     * @param query The SQL query to execute.
     * @param onlyFields A vector of field names to display in the results.
     * @param showSize Whether to display the size of the result set.
     * @param sizeOnly Whether to display only the size of the result set.
     */
    void executeQuery(const std::string& query, std::vector<std::string> onlyFields, bool showSize, bool sizeOnly);

    /**
     * @brief Processes a command entered by the user.
     * @param command The command string to process.
     */
    void processCommand(const std::string& command);

    /**
     * @brief Displays and manages the database menu interface.
     */
    void dataBaseMenu();

    /**
     * @brief Validates if a field exists in a specified table.
     * @param tableName The name of the table to check.
     * @param field The field name to validate.
     * @return True if the field is valid, false otherwise.
     */
    bool validateField(const std::string& tableName, const std::string& field);

    /**
     * @brief Executes a query for a specific table with given arguments.
     * @param table The name of the table to query.
     * @param args A vector of arguments to use in the query.
     */
    void executeQueryForTable(const std::string& table, const std::vector<std::string>& args);
};

#endif // DATABASEMENU_H