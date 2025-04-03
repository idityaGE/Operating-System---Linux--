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

//TODO: Need to fix this one
void writeResultsToFile(Process_detail* pd, int no_of_processes, Average_process_details* avg, const char* algorithm_name) {
  char filename[100];
  sprintf(filename, "results_%s.txt", algorithm_name);

  FILE* file = fopen(filename, "w");
  if (file == NULL) {
    printf("Error: Could not create file %s\n", filename);
    return;
  }

  fprintf(file, "=== Results for %s Scheduling Algorithm ===\n\n", algorithm_name);

  fprintf(file, "Process Details:\n");
  fprintf(file, "+---------+--------------+----------------+----------+--------------+---------------+------------------+-----------------+\n");
  fprintf(file, "| PID     | Arrival Time | Execution Time | Deadline | Waiting Time | Response Time | Turn Around Time | Completion Time |\n");
  fprintf(file, "+---------+--------------+----------------+----------+--------------+---------------+------------------+-----------------+\n");

  for (int i = 0; i < no_of_processes; i++) {
    fprintf(file, "| %-7d | %-12d | %-14d | %-8d | %-12d | %-13d | %-16d | %-15d |\n",
            pd[i].PID, pd[i].arrival_time, pd[i].execution_time, pd[i].deadline,
            pd[i].waiting_time, pd[i].response_time, pd[i].turn_around_time, pd[i].completion_time);
  }

  fprintf(file, "+---------+--------------+----------------+----------+--------------+---------------+------------------+-----------------+\n\n");

  fprintf(file, "Average Metrics:\n");
  fprintf(file, "+--------------------------+-------------------+\n");
  fprintf(file, "| Metric                   | Value             |\n");
  fprintf(file, "+--------------------------+-------------------+\n");
  fprintf(file, "| Average Waiting Time     | %-17d |\n", avg->average_waiting_time);
  fprintf(file, "| Average Response Time    | %-17d |\n", avg->average_response_time);
  fprintf(file, "| Average Turn Around Time | %-17d |\n", avg->average_turn_around_time);
  fprintf(file, "| CPU Utilization          | %-15d%% |\n", avg->CPU_utilization_time);
  fprintf(file, "| Total Time               | %-17d |\n", avg->total_time);
  fprintf(file, "+--------------------------+-------------------+\n");

  fclose(file);
  printf("\nResults have been written to %s\n", filename);
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
