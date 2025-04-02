#include "../include/display.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void clearScreen() {
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
}

void showOptions() {
  printf("Available Scheduling Algorithms:\n");
  printf("  1. First Come First Served (FCFS)\n");
  printf("  2. Round Robin (RR)\n");
  printf("  3. Shortest Job First (SJF)\n");
  printf("  4. Shortest Remaining Time Next (SRTN)\n");
  printf("  5. Highest Response Ratio Next (HRRN)\n");
  printf("  6. Lottery Scheduling\n");
  printf("  7. Earliest Deadline First (EDF)\n");
  printf("  8. Priority Scheduling\n");
  printf("  9. Run All Algorithms\n");
}

void displayProcessDetails(Process_detail* pd, int no_of_processes) {
  printf("+---------+--------------+----------------+----------+--------------+---------------+------------------+-----------------+\n");
  printf("| PID     | Arrival Time | Execution Time | Deadline | Waiting Time | Response Time | Turn Around Time | Completion Time |\n");
  printf("+---------+--------------+----------------+----------+--------------+---------------+------------------+-----------------+\n");

  for (int i = 0; i < no_of_processes; i++) {
    printf("| %-7d | %-12d | %-14d | %-8d | %-12d | %-13d | %-16d | %-15d |\n",
           pd[i].PID, pd[i].arrival_time, pd[i].execution_time, pd[i].deadline,
           pd[i].waiting_time, pd[i].response_time, pd[i].turn_around_time, pd[i].completion_time);
  }

  printf("+---------+--------------+----------------+----------+--------------+---------------+------------------+-----------------+\n");
}

void displayAverageDetails(Average_process_details* pd, char algorithm_name[]) {
  printf("\n");
  printf("+--------------------------+-------------------+\n");
  printf("| %-24s | %-17s |\n", "Scheduling Algorithm", algorithm_name);
  printf("+--------------------------+-------------------+\n");
  printf("| Average Waiting Time     | %-17d |\n", pd->average_waiting_time);
  printf("| Average Response Time    | %-17d |\n", pd->average_response_time);
  printf("| Average Turn Around Time | %-17d |\n", pd->average_turn_around_time);
  printf("| CPU Utilization          | %d%% \t       |\n", pd->CPU_utilization_time);
  printf("| Total Time               | %-17d |\n", pd->total_time);
  printf("+--------------------------+-------------------+\n");
}

void displayComparisonTable(Average_process_details** results, int count, char** algorithm_names) {
  printf("\n");
  printf("+----------------------+");
  for (int i = 0; i < count; i++) {
    printf("----------------+");
  }
  printf("\n| Metric              |");

  for (int i = 0; i < count; i++) {
    printf(" %-14s |", algorithm_names[i]);
  }

  printf("\n+----------------------+");
  for (int i = 0; i < count; i++) {
    printf("----------------+");
  }

  printf("\n| Avg Waiting Time     |");
  for (int i = 0; i < count; i++) {
    printf(" %-14d |", results[i]->average_waiting_time);
  }

  printf("\n| Avg Response Time    |");
  for (int i = 0; i < count; i++) {
    printf(" %-14d |", results[i]->average_response_time);
  }

  printf("\n| Avg Turn Around Time |");
  for (int i = 0; i < count; i++) {
    printf(" %-14d |", results[i]->average_turn_around_time);
  }

  printf("\n| CPU Utilization (%)  |");
  for (int i = 0; i < count; i++) {
    printf(" %-14d |", results[i]->CPU_utilization_time);
  }

  printf("\n| Total Time           |");
  for (int i = 0; i < count; i++) {
    printf(" %-14d |", results[i]->total_time);
  }

  printf("\n+----------------------+");
  for (int i = 0; i < count; i++) {
    printf("----------------+");
  }
  printf("\n");
}
