#include <limits.h>
#include <stdlib.h>
#include <time.h>

#include "../include/common.h"
#include "../include/display.h"

int getRandomNumber(int min, int max) {
  return min + rand() % (max - min + 1);
}

Average_process_details *Lottery(Process_detail *pd, int no_of_processes) {
  srand(time(NULL));

  Process_detail *pd_Lottery = (Process_detail *)calloc(no_of_processes, sizeof(Process_detail));
  if (pd_Lottery == NULL) {
    perror("Memory allocation failed");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < no_of_processes; i++)
    pd_Lottery[i] = pd[i];

  sortCol(pd_Lottery, no_of_processes, compareByArrivalTime);

  int current_time = 0;
  int total_execution_time = 0;
  int completed = 0;
  int *remaining_time = (int *)calloc(no_of_processes, sizeof(int));
  int *completed_processes = (int *)calloc(no_of_processes, sizeof(int));
  int *response_recorded = (int *)calloc(no_of_processes, sizeof(int));

  // Allocate tickets based on execution time 
  // This is just one possible ticket allocation policy
  int total_tickets = 100;
  int *tickets = (int *)calloc(no_of_processes, sizeof(int));

  if (!remaining_time || !completed_processes || !response_recorded || !tickets) {
    perror("Memory allocation failed");
    free(pd_Lottery);
    free(remaining_time);
    free(completed_processes);
    free(response_recorded);
    free(tickets);
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < no_of_processes; i++)
    remaining_time[i] = pd_Lottery[i].execution_time;

  Average_process_details *avg_details = (Average_process_details *)calloc(1, sizeof(Average_process_details));
  if (avg_details == NULL) {
    perror("Memory allocation failed");
    free(pd_Lottery);
    free(remaining_time);
    free(completed_processes);
    free(response_recorded);
    free(tickets);
    exit(EXIT_FAILURE);
  }

  if (pd_Lottery[0].arrival_time > 0)
    current_time = pd_Lottery[0].arrival_time;

  const int TIME_QUANTUM = 1;

  while (completed < no_of_processes) {
    int available_processes = 0;
    for (int i = 0; i < no_of_processes; i++) {
      if (pd_Lottery[i].arrival_time <= current_time && !completed_processes[i]) {
        available_processes++;
      }
    }

    if (available_processes == 0) {
      int next_arrival_time = INT_MAX;
      for (int i = 0; i < no_of_processes; i++) {
        if (!completed_processes[i] && pd_Lottery[i].arrival_time < next_arrival_time) {
          next_arrival_time = pd_Lottery[i].arrival_time;
        }
      }
      current_time = next_arrival_time;
      continue;
    }

    int tickets_per_process = total_tickets / available_processes;
    int ticket_count = 0;

    for (int i = 0; i < no_of_processes; i++) {
      if (pd_Lottery[i].arrival_time <= current_time && !completed_processes[i]) {
        tickets[i] = tickets_per_process;
        ticket_count += tickets_per_process;
      } else {
        tickets[i] = 0;
      }
    }

    int remaining_tickets = total_tickets - ticket_count;
    if (remaining_tickets > 0) {
      for (int i = 0; i < no_of_processes && remaining_tickets > 0; i++) {
        if (pd_Lottery[i].arrival_time <= current_time && !completed_processes[i]) {
          tickets[i]++;
          remaining_tickets--;
        }
      }
    }

    int winning_ticket = getRandomNumber(1, total_tickets);
    int selected_process = -1;
    int ticket_counter = 0;

    for (int i = 0; i < no_of_processes; i++) {
      ticket_counter += tickets[i];
      if (winning_ticket <= ticket_counter) {
        selected_process = i;
        break;
      }
    }

    if (selected_process == -1) {
      for (int i = 0; i < no_of_processes; i++) {
        if (pd_Lottery[i].arrival_time <= current_time && !completed_processes[i]) {
          selected_process = i;
          break;
        }
      }
    }

    if (!response_recorded[selected_process]) {
      pd_Lottery[selected_process].response_time = current_time - pd_Lottery[selected_process].arrival_time;
      response_recorded[selected_process] = 1;
    }

    int execution_time = TIME_QUANTUM;
    if (remaining_time[selected_process] < execution_time) {
      execution_time = remaining_time[selected_process];
    }

    remaining_time[selected_process] -= execution_time;
    current_time += execution_time;
    total_execution_time += execution_time;

    if (remaining_time[selected_process] == 0) {
      completed_processes[selected_process] = 1;
      completed++;

      pd_Lottery[selected_process].completion_time = current_time;
      pd_Lottery[selected_process].turn_around_time =
          pd_Lottery[selected_process].completion_time - pd_Lottery[selected_process].arrival_time;
      pd_Lottery[selected_process].waiting_time =
          pd_Lottery[selected_process].turn_around_time - pd_Lottery[selected_process].execution_time;

      avg_details->average_waiting_time += pd_Lottery[selected_process].waiting_time;
      avg_details->average_response_time += pd_Lottery[selected_process].response_time;
      avg_details->average_turn_around_time += pd_Lottery[selected_process].turn_around_time;
    }
  }

  avg_details->average_waiting_time /= no_of_processes;
  avg_details->average_response_time /= no_of_processes;
  avg_details->average_turn_around_time /= no_of_processes;

  avg_details->total_time = current_time;
  avg_details->CPU_utilization_time = (total_execution_time * 100) / current_time;

  displayProcessDetails(pd_Lottery, no_of_processes);

  free(pd_Lottery);
  free(remaining_time);
  free(completed_processes);
  free(response_recorded);
  free(tickets);
  return avg_details;
}
