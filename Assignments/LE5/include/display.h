#ifndef DISPLAY_H
#define DISPLAY_H

#include "common.h"

// Function to clear the screen
void clearScreen();

// Function to display menu options
void showOptions();

// Function to display process details
void displayProcessDetails(Process_detail* pd, int no_of_processes);

// Function to display average metrics for a scheduling algorithm
void displayAverageDetails(Average_process_details* pd, char algorithm_name[]);

// Function to display a comparison table of multiple scheduling algorithms
void displayComparisonTable(Average_process_details** results, int count, char** algorithm_names);

#endif  // DISPLAY_H
