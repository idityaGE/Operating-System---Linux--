#ifndef COMMON_H
#define COMMON_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int PID;
  int arrival_time;
  int execution_time;
  int deadline;
  int waiting_time;
  int response_time;
  int turn_around_time;
  int completion_time;
} Process_detail;

typedef struct {
  int average_waiting_time;
  int average_response_time;
  int average_turn_around_time;
} Average_process_details;

// Function declarations for common utilities
Process_detail* generateRandomProcessDetails(int no_of_processes);
int compareByArrivalTime(Process_detail a, Process_detail b);
int compareByExecutionTime(Process_detail a, Process_detail b);
int compareByDeadline(Process_detail a, Process_detail b);
void sortCol(Process_detail* pd_algo, int n, int (*compare)(Process_detail, Process_detail));

#endif  // COMMON_H
