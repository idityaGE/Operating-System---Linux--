#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <limits.h>
#include <stdbool.h>

#include "common.h"

// Function declarations for all scheduling algorithms
Average_process_details* FCFS(Process_detail* pd, int no_of_processes);
Average_process_details* RR(Process_detail* pd, int no_of_processes, int time_quantum);
Average_process_details* SJF(Process_detail* pd, int no_of_processes);
Average_process_details* SRTN(Process_detail* pd, int no_of_processes);
Average_process_details* HRRN(Process_detail* pd, int no_of_processes);
Average_process_details* Lottery(Process_detail* pd, int no_of_processes);
Average_process_details* EDF(Process_detail* pd, int no_of_processes);
Average_process_details* Priority(Process_detail* pd, int no_of_processes);

#endif  // ALGORITHMS_H
