#ifndef MENU_H
#define MENU_H

#include "BatchProcessor.h"
#include "DatabaseManager.h"

/**
 * @brief Displays the interactive main menu and handles user input.
 */
void showMenu();

/**
 * @brief Displays the configuration menu.
 */
void showConfigurationMenu();

/**
 * @brief Updates the database configuration settings.
 */
void updateDatabaseConfig();

#endif // MENU_H