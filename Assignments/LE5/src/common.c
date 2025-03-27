#include "../include/common.h"

Process_detail* generateRandomProcessDetails(int no_of_processes) {
  Process_detail* pd = (Process_detail*)calloc(no_of_processes, sizeof(Process_detail));
  if (pd == NULL) {
    perror("Memory allocation failed");
    exit(EXIT_FAILURE);
  }
  for (int i = 0; i < no_of_processes; i++) {
    pd[i].PID = rand() % (9999 - 99 + 1) + 99;
    pd[i].arrival_time = rand() % (99000 - 0 + 1) + 0;
    pd[i].execution_time = rand() % (9000 - 1 + 1) + 1;
    pd[i].deadline = rand() % (200 - 2 + 1) + 2;
  }
  return pd;
}

int compareByArrivalTime(Process_detail a, Process_detail b) {
  return a.arrival_time - b.arrival_time;
}

int compareByExecutionTime(Process_detail a, Process_detail b) {
  return a.execution_time - b.execution_time;
}

int compareByDeadline(Process_detail a, Process_detail b) {
  return a.deadline - b.deadline;
}

void sortCol(Process_detail* pd_algo, int n, int (*compare)(Process_detail, Process_detail)) {
  for (int i = 0; i < n - 1; i++) {
    bool isSwapped = false;
    for (int j = 0; j < n - i - 1; j++) {
      if (compare(pd_algo[j], pd_algo[j + 1]) > 0) {
        isSwapped = true;
        Process_detail temp = pd_algo[j];
        pd_algo[j] = pd_algo[j + 1];
        pd_algo[j + 1] = temp;
      }
    }
    if (!isSwapped) break;
  }
}
