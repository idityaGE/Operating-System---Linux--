#include <limits.h>

#include "../include/common.h"
#include "../include/display.h"

Average_process_details *RR(Process_detail *pd, int no_of_process, int time_quantum) {
  Process_detail *pd_RR = (Process_detail *)calloc(no_of_process, sizeof(Process_detail));
  if (pd_RR == NULL) {
    perror("Memory allocation failed");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < no_of_process; i++)
    pd_RR[i] = pd[i];

  sortCol(pd_RR, no_of_process, compareByArrivalTime);

  int current_time = 0;
  int total_execution_time = 0;
  int total_idle_time = 0;
  int last_completion_time = 0;

  Average_process_details *avg_details = (Average_process_details *)calloc(1, sizeof(Average_process_details));
  if (avg_details == NULL) {
    perror("Memory allocation failed");
    free(pd_RR);
    exit(EXIT_FAILURE);
  }

  if (pd_RR[0].arrival_time > 0) {
    total_idle_time += pd_RR[0].arrival_time;
    current_time = pd_RR[0].arrival_time;
  }

  int *remaining_time = (int *)calloc(no_of_process, sizeof(int));
  bool *first_response = (bool *)calloc(no_of_process, sizeof(bool));
  if (!remaining_time || !first_response) {
    perror("Memory allocation failed");
    free(pd_RR);
    free(avg_details);
    if (remaining_time) free(remaining_time);
    if (first_response) free(first_response);
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < no_of_process; i++) {
    remaining_time[i] = pd_RR[i].execution_time;
    first_response[i] = false;
  }

  int *ready_queue = (int *)calloc(no_of_process, sizeof(int));
  int queue_size = 0;

  for (int i = 0; i < no_of_process; i++)
    if (pd_RR[i].arrival_time <= current_time)
      ready_queue[queue_size++] = i;

  int completed = 0;
  while (completed < no_of_process) {
    if (queue_size == 0) {
      int next_arrival_time = INT_MAX;
      int next_process = -1;

      for (int i = 0; i < no_of_process; i++) {
        if (remaining_time[i] > 0 && pd_RR[i].arrival_time < next_arrival_time && pd_RR[i].arrival_time > current_time) {
          next_arrival_time = pd_RR[i].arrival_time;
          next_process = i;
        }
      }

      if (next_process != -1) {
        total_idle_time += (next_arrival_time - current_time);
        current_time = next_arrival_time;
        ready_queue[queue_size++] = next_process;
      } else
        break;
    }

    int current_process = ready_queue[0];

    for (int i = 0; i < queue_size - 1; i++)
      ready_queue[i] = ready_queue[i + 1];

    queue_size--;

    if (!first_response[current_process]) {
      pd_RR[current_process].response_time = current_time - pd_RR[current_process].arrival_time;
      first_response[current_process] = true;
    }

    int execution_slice = (remaining_time[current_process] < time_quantum) ? remaining_time[current_process] : time_quantum;

    current_time += execution_slice;
    remaining_time[current_process] -= execution_slice;
    total_execution_time += execution_slice;

    for (int i = 0; i < no_of_process; i++)
      if (remaining_time[i] > 0 &&
          pd_RR[i].arrival_time <= current_time &&
          i != current_process &&
          !isInQueue(ready_queue, queue_size, i))
        ready_queue[queue_size++] = i;

    if (remaining_time[current_process] > 0)
      ready_queue[queue_size++] = current_process;
    else {
      completed++;
      pd_RR[current_process].completion_time = current_time;
      pd_RR[current_process].turn_around_time = pd_RR[current_process].completion_time - pd_RR[current_process].arrival_time;
      pd_RR[current_process].waiting_time = pd_RR[current_process].turn_around_time - pd_RR[current_process].execution_time;

      if (pd_RR[current_process].completion_time > last_completion_time)
        last_completion_time = pd_RR[current_process].completion_time;

      avg_details->average_waiting_time += pd_RR[current_process].waiting_time;
      avg_details->average_response_time += pd_RR[current_process].response_time;
      avg_details->average_turn_around_time += pd_RR[current_process].turn_around_time;
    }
  }

  avg_details->average_waiting_time /= no_of_process;
  avg_details->average_response_time /= no_of_process;
  avg_details->average_turn_around_time /= no_of_process;

  avg_details->total_time = last_completion_time;
  avg_details->CPU_utilization_time = (total_execution_time * 100) / avg_details->total_time;

  displayProcessDetails(pd_RR, no_of_process);

  free(pd_RR);
  free(remaining_time);
  free(first_response);
  free(ready_queue);

  return avg_details;
}
