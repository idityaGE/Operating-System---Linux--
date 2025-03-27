#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void showOptions() {
  printf("Scheduling algorithm : \n\t(1) FCFS \n\t(2) RR \n\t(3) SJF \n\t(4) SRTN \n\t(5) HRRN \n\t(6) Lottery \n\t(7) EDF \n\t(8) Priority \n\t(9) All\n");
}

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

Process_detail* generateRandomProcessDetails(int no_of_processes) {
  Process_detail* pd = (Process_detail*)malloc(no_of_processes * sizeof(Process_detail));
  if (pd == NULL) {
    perror("Memory allocation failed");
    exit(EXIT_FAILURE);
  }
  for (int i = 0; i < no_of_processes; i++) {            // Time in milisecond (ms) := [ 1 sec = 1000 ms ]
    pd[i].PID = rand() % (9999 - 99 + 1) + 99;           // PID from 99 - 9999
    pd[i].arrival_time = rand() % (99000 - 0 + 1) + 0;   // 1ms - 99 sec
    pd[i].execution_time = rand() % (9000 - 1 + 1) + 1;  // 1ms - 9 sec
    pd[i].deadline = rand() % (200 - 2 + 1) + 2;         // 2ms - 200ms
  }
  return pd;
}

void displayProcessDetails(Process_detail* pd, int no_of_processes) {
  printf("+---------+--------------+----------------+----------+\n");
  printf("|   PID   | Arrival Time | Execution Time | Deadline |\n");
  printf("+---------+--------------+----------------+----------+\n");
  for (int i = 0; i < no_of_processes; i++) {
    printf("| %-7d | %-12d | %-14d | %-8d |\n", pd[i].PID, pd[i].arrival_time, pd[i].execution_time, pd[i].deadline);
  }
  printf("+---------+--------------+----------------+----------+\n");
}

Average_process_details* FCFS(Process_detail* pd, int no_of_processes) {
  
}

int main() {
  showOptions();
  int algo_no;
  printf("Enter select the Scheduling algorithm : ");
  scanf("%d", &algo_no);

  int no_of_processes;
  printf("Enter the number of Process : ");
  scanf("%d", &no_of_processes);

  Process_detail* pd = generateRandomProcessDetails(no_of_processes);

  displayProcessDetails(pd, no_of_processes);

  return 0;
}
