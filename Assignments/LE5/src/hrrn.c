#include <limits.h>

#include "../include/common.h"
#include "../include/display.h"

Average_process_details *HRRN(Process_detail *pd, int no_of_processes) {
  Process_detail *pd_HRRN = (Process_detail *)calloc(no_of_processes, sizeof(Process_detail));
  if (pd_HRRN == NULL) {
    perror("Memory allocation failed");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < no_of_processes; i++)
    pd_HRRN[i] = pd[i];

  sortCol(pd_HRRN, no_of_processes, compareByArrivalTime);

  int current_time = 0;
  int total_execution_time = 0;
  int completed = 0;
  int *completed_processes = (int *)calloc(no_of_processes, sizeof(int));

  if (completed_processes == NULL) {
    perror("Memory allocation failed");
    free(pd_HRRN);
    exit(EXIT_FAILURE);
  }

  Average_process_details *avg_details = (Average_process_details *)calloc(1, sizeof(Average_process_details));
  if (avg_details == NULL) {
    perror("Memory allocation failed");
    free(pd_HRRN);
    free(completed_processes);
    exit(EXIT_FAILURE);
  }

  if (pd_HRRN[0].arrival_time > 0)
    current_time = pd_HRRN[0].arrival_time;

  while (completed < no_of_processes) {
    int highest_ratio_idx = -1;
    float highest_ratio = -1.0;

    for (int i = 0; i < no_of_processes; i++) {
      if (pd_HRRN[i].arrival_time <= current_time && !completed_processes[i]) {
        int waiting_time = current_time - pd_HRRN[i].arrival_time;

        float response_ratio = (float)(waiting_time + pd_HRRN[i].execution_time) / pd_HRRN[i].execution_time;

        if (response_ratio > highest_ratio) {
          highest_ratio = response_ratio;
          highest_ratio_idx = i;
        }
      }
    }

    if (highest_ratio_idx == -1) {
      int next_arrival_time = INT_MAX;
      for (int i = 0; i < no_of_processes; i++) {
        if (!completed_processes[i] && pd_HRRN[i].arrival_time < next_arrival_time)
          next_arrival_time = pd_HRRN[i].arrival_time;
      }
      current_time = next_arrival_time;
      continue;
    }

    pd_HRRN[highest_ratio_idx].response_time = current_time - pd_HRRN[highest_ratio_idx].arrival_time;
    pd_HRRN[highest_ratio_idx].waiting_time = pd_HRRN[highest_ratio_idx].response_time;

    current_time += pd_HRRN[highest_ratio_idx].execution_time;
    pd_HRRN[highest_ratio_idx].completion_time = current_time;

    pd_HRRN[highest_ratio_idx].turn_around_time =
        pd_HRRN[highest_ratio_idx].completion_time - pd_HRRN[highest_ratio_idx].arrival_time;

    total_execution_time += pd_HRRN[highest_ratio_idx].execution_time;
    completed_processes[highest_ratio_idx] = 1;
    completed++;

    avg_details->average_waiting_time += pd_HRRN[highest_ratio_idx].waiting_time;
    avg_details->average_response_time += pd_HRRN[highest_ratio_idx].response_time;
    avg_details->average_turn_around_time += pd_HRRN[highest_ratio_idx].turn_around_time;
  }

  avg_details->average_waiting_time /= no_of_processes;
  avg_details->average_response_time /= no_of_processes;
  avg_details->average_turn_around_time /= no_of_processes;

  avg_details->total_time = current_time;
  avg_details->CPU_utilization_time = (total_execution_time * 100) / current_time;

  displayProcessDetails(pd_HRRN, no_of_processes);

  free(pd_HRRN);
  free(completed_processes);
  return avg_details;
}
