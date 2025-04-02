#include "include/algorithms.h"
#include "include/common.h"
#include "include/display.h"

int main() {
  showOptions();
  int algo_no;
  printf("Enter select the Scheduling algorithm : ");
  scanf("%d", &algo_no);

  int no_of_processes;
  printf("Enter the number of Process : ");
  scanf("%d", &no_of_processes);

  Process_detail* pd = generateRandomProcessDetails(no_of_processes);

  displayProcessDetails(pd, no_of_processes);

  Average_process_details* res = NULL;

  // res = FCFS(pd, no_of_processes);
  // displayAverageDetails(res, "FCFS");

  res = RR(pd, no_of_processes, 100);
  displayAverageDetails(res, "Round Robin");

  free(pd);
  if (res) free(res);

  return 0;
}
