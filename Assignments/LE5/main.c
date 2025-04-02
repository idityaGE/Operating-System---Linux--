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
        char save_choice;
        printf("\nDo you want to save these results to a file? (y/n): ");
        getchar(); // Clear input buffer
        scanf("%c", &save_choice);
        
        if (save_choice == 'y' || save_choice == 'Y') {
            writeResultsToFile(pd, no_of_processes, res, 
                algo_no == 1 ? "FCFS" : 
                algo_no == 2 ? "RoundRobin" : 
                algo_no == 3 ? "SJF" : 
                algo_no == 4 ? "SRTN" : 
                algo_no == 5 ? "HRRN" : 
                algo_no == 6 ? "Lottery" : 
                algo_no == 7 ? "EDF" : "Priority");
            getchar(); // Clear input buffer
        }
        
        free(res);
    } else {
        // For "Run All" option
        char save_choice;
        printf("\nDo you want to save all results to files? (y/n): ");
        getchar(); // Clear input buffer
        scanf("%c", &save_choice);
        
        if (save_choice == 'y' || save_choice == 'Y') {
            printf("\nSaving all results to separate files...\n");
            writeResultsToFile(pd, no_of_processes, res, "Priority");
        }
        
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
