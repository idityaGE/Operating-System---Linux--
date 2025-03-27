#ifndef DISPLAY_H
#define DISPLAY_H

#include "common.h"

void showOptions();
void displayProcessDetails(Process_detail* pd, int no_of_processes);
void displayAverageDetails(Average_process_details* pd, char algorithm_name[]);

#endif  // DISPLAY_H
