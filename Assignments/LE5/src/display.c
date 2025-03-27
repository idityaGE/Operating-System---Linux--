#include "../include/display.h"

void showOptions() {
  printf("Scheduling algorithm : \n\t(1) FCFS \n\t(2) RR \n\t(3) SJF \n\t(4) SRTN \n\t(5) HRRN \n\t(6) Lottery \n\t(7) EDF \n\t(8) Priority \n\t(9) All\n");
}

void displayProcessDetails(Process_detail* pd, int no_of_processes) {
  printf("+---------+--------------+----------------+----------+--------------+---------------+------------------+-----------------+\n");
  printf("|   PID   | Arrival Time | Execution Time | Deadline | Waiting Time | Response Time | Turn Around Time | Completion Time |\n");
  printf("+---------+--------------+----------------+----------+--------------+---------------+------------------+-----------------+\n");
  for (int i = 0; i < no_of_processes; i++) {
    printf("| %-7d | %-12d | %-14d | %-8d | %-12d | %-13d | %-16d | %-15d |\n",
           pd[i].PID, pd[i].arrival_time, pd[i].execution_time, pd[i].deadline,
           pd[i].waiting_time, pd[i].response_time, pd[i].turn_around_time, pd[i].completion_time);
  }
  printf("+---------+--------------+----------------+----------+--------------+---------------+------------------+-----------------+\n");
}

void displayAverageDetails(Average_process_details* pd, char algorithm_name[]) {
  printf("+--------------------------+-------------------+\n");
  printf("| Scheduling Algorithm     | %-17s |\n", algorithm_name);
  printf("+--------------------------+-------------------+\n");
  printf("| Average Waiting Time     | %-17d |\n", pd->average_waiting_time);
  printf("| Average Response Time    | %-17d |\n", pd->average_response_time);
  printf("| Average Turn Around Time | %-17d |\n", pd->average_turn_around_time);
  printf("| CPU Utilization          | %d%% \t       |\n", pd->CPU_utilization_time);
  printf("| Total Time               | %-17d |\n", pd->total_time);
  printf("+--------------------------+-------------------+\n");
}
