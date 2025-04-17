#include <limits.h>

#include "../include/common.h"
#include "../include/display.h"

Average_process_details *Priority(Process_detail *pd, int no_of_processes) {
  Process_detail *pd_Priority = (Process_detail *)calloc(no_of_processes, sizeof(Process_detail));
  if (pd_Priority == NULL) {
    perror("Memory allocation failed");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < no_of_processes; i++)
    pd_Priority[i] = pd[i];

  sortCol(pd_Priority, no_of_processes, compareByArrivalTime);

  int current_time = 0;
  int total_execution_time = 0;
  int completed = 0;
  int *completed_processes = (int *)calloc(no_of_processes, sizeof(int));

  if (completed_processes == NULL) {
    perror("Memory allocation failed");
    free(pd_Priority);
    exit(EXIT_FAILURE);
  }

  Average_process_details *avg_details = (Average_process_details *)calloc(1, sizeof(Average_process_details));
  if (avg_details == NULL) {
    perror("Memory allocation failed");
    free(pd_Priority);
    free(completed_processes);
    exit(EXIT_FAILURE);
  }

  if (pd_Priority[0].arrival_time > 0)
    current_time = pd_Priority[0].arrival_time;

  while (completed < no_of_processes) {
    int highest_priority_idx = -1;
    int highest_priority = INT_MAX;

    for (int i = 0; i < no_of_processes; i++) {
      if (pd_Priority[i].arrival_time <= current_time && !completed_processes[i] &&
          pd_Priority[i].deadline < highest_priority) {
        highest_priority = pd_Priority[i].deadline;  // Using deadline as priority
        highest_priority_idx = i;
      }
    }

    if (highest_priority_idx == -1) {
      int next_arrival_time = INT_MAX;
      for (int i = 0; i < no_of_processes; i++) {
        if (!completed_processes[i] && pd_Priority[i].arrival_time < next_arrival_time)
          next_arrival_time = pd_Priority[i].arrival_time;
      }
      current_time = next_arrival_time;
      continue; // skipping and restarting with new process
    }

    pd_Priority[highest_priority_idx].response_time = current_time - pd_Priority[highest_priority_idx].arrival_time;
    pd_Priority[highest_priority_idx].waiting_time = pd_Priority[highest_priority_idx].response_time;

    current_time += pd_Priority[highest_priority_idx].execution_time;
    pd_Priority[highest_priority_idx].completion_time = current_time;

    pd_Priority[highest_priority_idx].turn_around_time =
        pd_Priority[highest_priority_idx].completion_time - pd_Priority[highest_priority_idx].arrival_time;

    total_execution_time += pd_Priority[highest_priority_idx].execution_time;
    completed_processes[highest_priority_idx] = 1;
    completed++;

    avg_details->average_waiting_time += pd_Priority[highest_priority_idx].waiting_time;
    avg_details->average_response_time += pd_Priority[highest_priority_idx].response_time;
    avg_details->average_turn_around_time += pd_Priority[highest_priority_idx].turn_around_time;
  }

  avg_details->average_waiting_time /= no_of_processes;
  avg_details->average_response_time /= no_of_processes;
  avg_details->average_turn_around_time /= no_of_processes;

  avg_details->total_time = current_time;
  avg_details->CPU_utilization_time = (total_execution_time * 100) / current_time;

  displayProcessDetails(pd_Priority, no_of_processes);

  free(pd_Priority);
  free(completed_processes);
  return avg_details;
}
