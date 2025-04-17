#ifndef DISPLAY_H
#define DISPLAY_H

#include "common.h"

void clearScreen();

void showOptions();

void displayProcessDetails(Process_detail* pd, int no_of_processes);

void displayAverageDetails(Average_process_details* pd, char algorithm_name[]);

void writeResultsToFile(Process_detail* pd, int no_of_processes, Average_process_details* avg, const char* algorithm_name);

#endif  // DISPLAY_H
