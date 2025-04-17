# Lab Experiment 5

## Objective

Write a simulator to implement and analyze different CPU scheduling algorithms. The simulator should provide the following utilization metrics:

1. **CPU Utilization**
2. **Waiting Time**:
  - Waiting time of each process
  - Average waiting time
3. **Response Time**:
  - Response time of each process
  - Average response time
4. **Turnaround Time**:
  - Turnaround time of each process
  - Average turnaround time

## Input Parameters

The simulator will accept two arguments:
1. **Scheduling Algorithm**: Specify the algorithm to simulate.
2. **Number of Processes**: Define the number of processes to simulate.

Based on these inputs, the simulator will randomly generate the following parameters:
- **Arrival Time**
- **CPU Burst (Execution Time)**
- **Deadline**

## Supported Scheduling Algorithms

The simulator will support the following scheduling algorithms:
1. **First-Come, First-Served (FCFS)**
2. **Round Robin (RR)**
3. **Shortest Job First (SJF)**
4. **Shortest Remaining Time Next (SRTN)**
5. **Highest Response Ratio Next (HRRN)**
6. **Lottery Scheduling**
7. **Earliest Deadline First (EDF)**
8. **Priority Scheduling**
9. **All Algorithms** (Run all algorithms using the same data)

You may also include additional algorithms if desired.

## Key Features

1. **Random Number Generation**:
  - Use a near-perfect random number generator for generating process parameters.
2. **Algorithm-Specific Considerations**:
  - For **SJF**, assume all jobs arrive at the start (no arrival time needed).
  - For **RR**, provide results for multiple quantum sizes.
  - Show results for both **Preemptive** and **Non-Preemptive** variants of applicable algorithms.
3. **Output**:
  - Results can be displayed on **STDOUT** or written to an output file (`Output.tex`).
  - Use a separate file (`process_data.tex`) to store randomly generated process data.

## Example Run

```plaintext
CU Raj gaurav@curaj.ac.in
root# CPUSimulator
Hi! Welcome to CPU Scheduling Simulator. Please give me required parameters.

Scheduling algorithm – (1) FCFS (2) RR (3) SJF (4) SRTN (5) HRRN (6) Lottery (7) EDF (8) Priority (9) All
ALL

No. of Processes in the system: 50
Wait.... Generating Schedules...
DONE. Please check output file Output.tex for all the results.
```

## Additional Features

1. **Gantt Charts**: Visualize the scheduling timeline for each algorithm.
2. **Comparison Table**: Provide a summary table comparing all algorithms at the end.

