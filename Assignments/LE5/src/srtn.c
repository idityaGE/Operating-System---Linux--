#include <limits.h>

#include "../include/common.h"
#include "../include/display.h"

Average_process_details *SRTN(Process_detail *pd, int no_of_processes) {
  Process_detail *pd_SRTN = (Process_detail *)calloc(no_of_processes, sizeof(Process_detail));
  if (pd_SRTN == NULL) {
    perror("Memory allocation failed");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < no_of_processes; i++)
    pd_SRTN[i] = pd[i];

  sortCol(pd_SRTN, no_of_processes, compareByArrivalTime);

  int current_time = 0;
  int total_execution_time = 0;
  int completed = 0;
  int *completed_processes = (int *)calloc(no_of_processes, sizeof(int));
  int *remaining_time = (int *)calloc(no_of_processes, sizeof(int));
  int *response_recorded = (int *)calloc(no_of_processes, sizeof(int));

  if (!completed_processes || !remaining_time || !response_recorded) {
    perror("Memory allocation failed");
    free(pd_SRTN);
    free(completed_processes);
    free(remaining_time);
    free(response_recorded);
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < no_of_processes; i++)
    remaining_time[i] = pd_SRTN[i].execution_time;

  Average_process_details *avg_details = (Average_process_details *)calloc(1, sizeof(Average_process_details));
  if (avg_details == NULL) {
    perror("Memory allocation failed");
    free(pd_SRTN);
    free(completed_processes);
    free(remaining_time);
    free(response_recorded);
    exit(EXIT_FAILURE);
  }

  if (pd_SRTN[0].arrival_time > 0)
    current_time = pd_SRTN[0].arrival_time;

  while (completed < no_of_processes) {
    int smallest_remaining_idx = -1;
    int smallest_remaining_time = INT_MAX;

    for (int i = 0; i < no_of_processes; i++) {
      if (pd_SRTN[i].arrival_time <= current_time &&
          !completed_processes[i] &&
          remaining_time[i] < smallest_remaining_time) {
        smallest_remaining_time = remaining_time[i];
        smallest_remaining_idx = i;
      }
    }

    if (smallest_remaining_idx == -1) {
      int next_arrival_time = INT_MAX;
      for (int i = 0; i < no_of_processes; i++)
        if (!completed_processes[i] && pd_SRTN[i].arrival_time < next_arrival_time)
          next_arrival_time = pd_SRTN[i].arrival_time;
      current_time = next_arrival_time;
      continue;
    }

    if (!response_recorded[smallest_remaining_idx]) {
      pd_SRTN[smallest_remaining_idx].response_time = current_time - pd_SRTN[smallest_remaining_idx].arrival_time;
      response_recorded[smallest_remaining_idx] = 1;
    }

    int run_time = 1;

    int next_event_time = INT_MAX;
    for (int i = 0; i < no_of_processes; i++) {
      if (!completed_processes[i] && pd_SRTN[i].arrival_time > current_time) {
        next_event_time = pd_SRTN[i].arrival_time < next_event_time ? pd_SRTN[i].arrival_time : next_event_time;
      }
    }

    if (next_event_time == INT_MAX ||
        current_time + remaining_time[smallest_remaining_idx] <= next_event_time)
      run_time = remaining_time[smallest_remaining_idx];
    else
      run_time = next_event_time - current_time;

    remaining_time[smallest_remaining_idx] -= run_time;
    current_time += run_time;
    total_execution_time += run_time;

    if (remaining_time[smallest_remaining_idx] == 0) {
      completed++;
      completed_processes[smallest_remaining_idx] = 1;
      pd_SRTN[smallest_remaining_idx].completion_time = current_time;
      pd_SRTN[smallest_remaining_idx].turn_around_time =
          pd_SRTN[smallest_remaining_idx].completion_time - pd_SRTN[smallest_remaining_idx].arrival_time;
      pd_SRTN[smallest_remaining_idx].waiting_time =
          pd_SRTN[smallest_remaining_idx].turn_around_time - pd_SRTN[smallest_remaining_idx].execution_time;

      avg_details->average_waiting_time += pd_SRTN[smallest_remaining_idx].waiting_time;
      avg_details->average_response_time += pd_SRTN[smallest_remaining_idx].response_time;
      avg_details->average_turn_around_time += pd_SRTN[smallest_remaining_idx].turn_around_time;
    }
  }

  avg_details->average_waiting_time /= no_of_processes;
  avg_details->average_response_time /= no_of_processes;
  avg_details->average_turn_around_time /= no_of_processes;

  avg_details->total_time = current_time;
  avg_details->CPU_utilization_time = (total_execution_time * 100) / current_time;

  displayProcessDetails(pd_SRTN, no_of_processes);

  free(pd_SRTN);
  free(completed_processes);
  free(remaining_time);
  free(response_recorded);
  return avg_details;
}
