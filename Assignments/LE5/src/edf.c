#include <limits.h>

#include "../include/common.h"
#include "../include/display.h"

Average_process_details *EDF(Process_detail *pd, int no_of_processes) {
  Process_detail *pd_EDF = (Process_detail *)calloc(no_of_processes, sizeof(Process_detail));
  if (pd_EDF == NULL) {
    perror("Memory allocation failed");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < no_of_processes; i++)
    pd_EDF[i] = pd[i];

  sortCol(pd_EDF, no_of_processes, compareByArrivalTime);

  int current_time = 0;
  int total_execution_time = 0;
  int completed = 0;
  int *remaining_time = (int *)calloc(no_of_processes, sizeof(int));
  int *completed_processes = (int *)calloc(no_of_processes, sizeof(int));
  int *response_recorded = (int *)calloc(no_of_processes, sizeof(int));
  int *absolute_deadlines = (int *)calloc(no_of_processes, sizeof(int));

  if (!remaining_time || !completed_processes || !response_recorded || !absolute_deadlines) {
    perror("Memory allocation failed");
    free(pd_EDF);
    free(remaining_time);
    free(completed_processes);
    free(response_recorded);
    free(absolute_deadlines);
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < no_of_processes; i++) {
    remaining_time[i] = pd_EDF[i].execution_time;
    absolute_deadlines[i] = pd_EDF[i].arrival_time + pd_EDF[i].deadline;
  }

  Average_process_details *avg_details = (Average_process_details *)calloc(1, sizeof(Average_process_details));
  if (avg_details == NULL) {
    perror("Memory allocation failed");
    free(pd_EDF);
    free(remaining_time);
    free(completed_processes);
    free(response_recorded);
    free(absolute_deadlines);
    exit(EXIT_FAILURE);
  }

  if (pd_EDF[0].arrival_time > 0)
    current_time = pd_EDF[0].arrival_time;

  while (completed < no_of_processes) {
    int earliest_deadline_idx = -1;
    int earliest_deadline = INT_MAX;

    for (int i = 0; i < no_of_processes; i++) {
      if (pd_EDF[i].arrival_time <= current_time && !completed_processes[i] &&
          absolute_deadlines[i] < earliest_deadline) {
        earliest_deadline = absolute_deadlines[i];
        earliest_deadline_idx = i;
      }
    }

    if (earliest_deadline_idx == -1) {
      int next_arrival_time = INT_MAX;
      for (int i = 0; i < no_of_processes; i++) {
        if (!completed_processes[i] && pd_EDF[i].arrival_time < next_arrival_time) {
          next_arrival_time = pd_EDF[i].arrival_time;
        }
      }
      current_time = next_arrival_time;
      continue;
    }

    if (!response_recorded[earliest_deadline_idx]) {
      pd_EDF[earliest_deadline_idx].response_time = current_time - pd_EDF[earliest_deadline_idx].arrival_time;
      response_recorded[earliest_deadline_idx] = 1;
    }

    int next_arrival_time = INT_MAX;
    for (int i = 0; i < no_of_processes; i++) {
      if (!completed_processes[i] && pd_EDF[i].arrival_time > current_time &&
          pd_EDF[i].arrival_time < next_arrival_time) {
        next_arrival_time = pd_EDF[i].arrival_time;
      }
    }

    int run_time = remaining_time[earliest_deadline_idx];
    if (next_arrival_time != INT_MAX) {
      run_time = (next_arrival_time - current_time < run_time) ? (next_arrival_time - current_time) : run_time;
    }

    remaining_time[earliest_deadline_idx] -= run_time;
    current_time += run_time;
    total_execution_time += run_time;

    if (remaining_time[earliest_deadline_idx] == 0) {
      completed++;
      completed_processes[earliest_deadline_idx] = 1;
      pd_EDF[earliest_deadline_idx].completion_time = current_time;
      pd_EDF[earliest_deadline_idx].turn_around_time =
          pd_EDF[earliest_deadline_idx].completion_time - pd_EDF[earliest_deadline_idx].arrival_time;
      pd_EDF[earliest_deadline_idx].waiting_time =
          pd_EDF[earliest_deadline_idx].turn_around_time - pd_EDF[earliest_deadline_idx].execution_time;

      // Check if deadline was met
      if (pd_EDF[earliest_deadline_idx].completion_time > absolute_deadlines[earliest_deadline_idx]) {
        // You could log this or handle missed deadlines differently
        // For now, we'll just calculate the metrics the same way
      }

      avg_details->average_waiting_time += pd_EDF[earliest_deadline_idx].waiting_time;
      avg_details->average_response_time += pd_EDF[earliest_deadline_idx].response_time;
      avg_details->average_turn_around_time += pd_EDF[earliest_deadline_idx].turn_around_time;
    }
  }

  avg_details->average_waiting_time /= no_of_processes;
  avg_details->average_response_time /= no_of_processes;
  avg_details->average_turn_around_time /= no_of_processes;

  avg_details->total_time = current_time;
  avg_details->CPU_utilization_time = (total_execution_time * 100) / current_time;

  displayProcessDetails(pd_EDF, no_of_processes);

  free(pd_EDF);
  free(remaining_time);
  free(completed_processes);
  free(response_recorded);
  free(absolute_deadlines);
  return avg_details;
}
