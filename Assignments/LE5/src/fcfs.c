#include "../include/common.h"
#include "../include/display.h"

Average_process_details* FCFS(Process_detail* pd, int no_of_processes) {
  Process_detail* pd_FCFS = (Process_detail*)calloc(no_of_processes, sizeof(Process_detail));
  if (pd_FCFS == NULL) {
    perror("Memory allocation failed");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < no_of_processes; i++)
    pd_FCFS[i] = pd[i];
  sortCol(pd_FCFS, no_of_processes, compareByArrivalTime);

  int current_time = 0;
  int total_execution_time = 0;
  int total_idle_time = 0;
  int last_completion_time = 0;

  Average_process_details* avg_details = (Average_process_details*)calloc(1, sizeof(Average_process_details));
  if (avg_details == NULL) {
    perror("Memory allocation failed");
    free(pd_FCFS);
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < no_of_processes; i++) {
    if (current_time < pd_FCFS[i].arrival_time) {
      total_idle_time += (pd_FCFS[i].arrival_time - current_time);
      current_time = pd_FCFS[i].arrival_time;
    }

    pd_FCFS[i].response_time = current_time - pd_FCFS[i].arrival_time;
    pd_FCFS[i].waiting_time = pd_FCFS[i].response_time;
    pd_FCFS[i].completion_time = current_time + pd_FCFS[i].execution_time;
    pd_FCFS[i].turn_around_time = pd_FCFS[i].completion_time - pd_FCFS[i].arrival_time;

    current_time += pd_FCFS[i].execution_time;
    total_execution_time += pd_FCFS[i].execution_time;

    if (pd_FCFS[i].completion_time > last_completion_time) {
      last_completion_time = pd_FCFS[i].completion_time;
    }

    avg_details->average_waiting_time += pd_FCFS[i].waiting_time;
    avg_details->average_response_time += pd_FCFS[i].response_time;
    avg_details->average_turn_around_time += pd_FCFS[i].turn_around_time;
  }

  avg_details->average_waiting_time /= no_of_processes;
  avg_details->average_response_time /= no_of_processes;
  avg_details->average_turn_around_time /= no_of_processes;

  avg_details->total_time = last_completion_time;

  avg_details->CPU_utilization_time = (total_execution_time * 100) / avg_details->total_time;

  displayProcessDetails(pd_FCFS, no_of_processes);

  free(pd_FCFS);
  return avg_details;
}
