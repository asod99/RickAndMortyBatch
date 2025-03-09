#ifndef MENU_H
#define MENU_H

#include "BatchProcessor.h"
#include "DatabaseManager.h"

/**
 * @brief Displays the interactive menu and handles user input.
 * @param batchProcessor Reference to the BatchProcessor.
 * @param dbManager Reference to the DatabaseManager.
 */
void showMenu();

void showConfigurationMenu();

void updateDatabaseConfig();

#endif // MENU_H