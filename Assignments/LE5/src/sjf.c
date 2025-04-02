#include <limits.h>

#include "../include/common.h"
#include "../include/display.h"

Average_process_details *SJF(Process_detail *pd, int no_of_process) {
  Process_detail *pd_SJF = (Process_detail *)calloc(no_of_process, sizeof(Process_detail));
  if (pd_SJF == NULL) {
    perror("Memory allocation failed");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < no_of_process; i++)
    pd_SJF[i] = pd[i];

  sortCol(pd_SJF, no_of_process, compareByArrivalTime);

  int current_time = 0;
  int total_execution_time = 0;
  int completed = 0;
  int *completed_processes = (int *)calloc(no_of_process, sizeof(int));

  if (completed_processes == NULL) {
    perror("Memory allocation failed");
    free(pd_SJF);
    exit(EXIT_FAILURE);
  }

  Average_process_details *avg_details = (Average_process_details *)calloc(1, sizeof(Average_process_details));
  if (avg_details == NULL) {
    perror("Memory allocation failed");
    free(pd_SJF);
    free(completed_processes);
    exit(EXIT_FAILURE);
  }

  if (pd_SJF[0].arrival_time > 0)
    current_time = pd_SJF[0].arrival_time;

  while (completed < no_of_process) {
    int shortest_job_idx = -1;
    int shortest_burst_time = INT_MAX;

    for (int i = 0; i < no_of_process; i++) {
      if (pd_SJF[i].arrival_time <= current_time && !completed_processes[i] &&
          pd_SJF[i].execution_time < shortest_burst_time) {
        shortest_burst_time = pd_SJF[i].execution_time;
        shortest_job_idx = i;
      }
    }

    if (shortest_job_idx == -1) {
      int next_arrival_time = INT_MAX;
      for (int i = 0; i < no_of_process; i++)
        if (!completed_processes[i] && pd_SJF[i].arrival_time < next_arrival_time)
          next_arrival_time = pd_SJF[i].arrival_time;
      current_time = next_arrival_time;
      continue;
    }

    pd_SJF[shortest_job_idx].response_time = current_time - pd_SJF[shortest_job_idx].arrival_time;
    pd_SJF[shortest_job_idx].waiting_time = pd_SJF[shortest_job_idx].response_time;
    current_time += pd_SJF[shortest_job_idx].execution_time;
    pd_SJF[shortest_job_idx].completion_time = current_time;
    pd_SJF[shortest_job_idx].turn_around_time = pd_SJF[shortest_job_idx].completion_time -
                                                pd_SJF[shortest_job_idx].arrival_time;

    total_execution_time += pd_SJF[shortest_job_idx].execution_time;
    completed_processes[shortest_job_idx] = 1;
    completed++;

    avg_details->average_waiting_time += pd_SJF[shortest_job_idx].waiting_time;
    avg_details->average_response_time += pd_SJF[shortest_job_idx].response_time;
    avg_details->average_turn_around_time += pd_SJF[shortest_job_idx].turn_around_time;
  }

  avg_details->average_waiting_time /= no_of_process;
  avg_details->average_response_time /= no_of_process;
  avg_details->average_turn_around_time /= no_of_process;

  avg_details->total_time = current_time;
  avg_details->CPU_utilization_time = (total_execution_time * 100) / current_time;

  displayProcessDetails(pd_SJF, no_of_process);

  free(pd_SJF);
  free(completed_processes);
  return avg_details;
}
