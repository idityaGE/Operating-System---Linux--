#include "include/algorithms.h"
#include "include/common.h"
#include "include/display.h"

int main() {
  int algo_no;
  int no_of_processes;
  bool run_again = true;

  while (run_again) {
    clearScreen();
    printf("\n===== CPU Scheduling Algorithm Simulator =====\n\n");
    showOptions();

    printf("\nEnter the scheduling algorithm number: ");
    scanf("%d", &algo_no);

    if (algo_no < 1 || algo_no > 9) {
      printf("Invalid choice! Please select a number between 1 and 9.\n");
      printf("Press Enter to continue...");
      getchar();  // Clear input buffer
      getchar();  // Wait for Enter key
      continue;
    }

    printf("Enter the number of processes: ");
    scanf("%d", &no_of_processes);

    if (no_of_processes <= 0) {
      printf("Number of processes must be positive!\n");
      printf("Press Enter to continue...");
      getchar();  // Clear input buffer
      getchar();  // Wait for Enter key
      continue;
    }

    Process_detail* pd = generateRandomProcessDetails(no_of_processes);
    Average_process_details* res = NULL;

    printf("\n--- Initial Process Details ---\n");
    displayProcessDetails(pd, no_of_processes);

    printf("\n--- Scheduling Results ---\n");

    switch (algo_no) {
      case 1:
        res = FCFS(pd, no_of_processes);
        displayAverageDetails(res, "FCFS");
        break;
      case 2: {
        int time_quantum;
        printf("Enter time quantum for Round Robin: ");
        scanf("%d", &time_quantum);
        res = RR(pd, no_of_processes, time_quantum);
        displayAverageDetails(res, "Round Robin");
      } break;
      case 3:
        res = SJF(pd, no_of_processes);
        displayAverageDetails(res, "SJF");
        break;
      case 4:
        res = SRTN(pd, no_of_processes);
        displayAverageDetails(res, "SRTN");
        break;
      case 5:
        res = HRRN(pd, no_of_processes);
        displayAverageDetails(res, "HRRN");
        break;
      case 6:
        res = Lottery(pd, no_of_processes);
        displayAverageDetails(res, "Lottery");
        break;
      case 7:
        res = EDF(pd, no_of_processes);
        displayAverageDetails(res, "EDF");
        break;
      case 8:
        res = Priority(pd, no_of_processes);
        displayAverageDetails(res, "Priority");
        break;
      case 9:
        printf("\n--- Running All Scheduling Algorithms ---\n\n");

        res = FCFS(pd, no_of_processes);
        displayAverageDetails(res, "FCFS");
        free(res);

        int time_quantum;
        printf("\nEnter time quantum for Round Robin: ");
        scanf("%d", &time_quantum);
        res = RR(pd, no_of_processes, time_quantum);
        displayAverageDetails(res, "Round Robin");
        free(res);

        res = SJF(pd, no_of_processes);
        displayAverageDetails(res, "SJF");
        free(res);

        res = SRTN(pd, no_of_processes);
        displayAverageDetails(res, "SRTN");
        free(res);

        res = HRRN(pd, no_of_processes);
        displayAverageDetails(res, "HRRN");
        free(res);

        res = Lottery(pd, no_of_processes);
        displayAverageDetails(res, "Lottery");
        free(res);

        res = EDF(pd, no_of_processes);
        displayAverageDetails(res, "EDF");
        free(res);

        res = Priority(pd, no_of_processes);
        displayAverageDetails(res, "Priority");
        break;
    }

    if (algo_no != 9) {
      free(res);
    }
    free(pd);

    char choice;
    printf("\nDo you want to run another simulation? (y/n): ");
    getchar();  // Clear input buffer
    scanf("%c", &choice);

    run_again = (choice == 'y' || choice == 'Y');
  }

  printf("\nThank you for using the CPU Scheduling Simulator!\n");
  return 0;
}
