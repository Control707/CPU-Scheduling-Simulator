#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <float.h>

#define MAX_PROCESSES 100
#define MAX_QUEUES 5

// Process Control Block structure
typedef struct {
    int pid;
    int arrival_time;
    int burst_time;
    int priority;
    int remaining_time;
    int waiting_time;
    int turnaround_time;
    int completion_time;
    int response_time;
    bool first_response;
    int queue_level;  // For MLFQ
} Process;

// Performance Metrics structure
typedef struct {
    double avg_waiting_time;
    double avg_turnaround_time;
    double avg_response_time;
    double cpu_utilization;
    double throughput;
    int total_time;
} Metrics;

// Function prototypes
void display_menu();
void input_processes(Process processes[], int *n);
void generate_test_workload(Process processes[], int *n, int type);
void fcfs(Process processes[], int n, Metrics *metrics);
void sjf(Process processes[], int n, Metrics *metrics);
void priority_scheduling(Process processes[], int n, Metrics *metrics);
void round_robin(Process processes[], int n, int quantum, Metrics *metrics);
void srtf(Process processes[], int n, Metrics *metrics);
void mlfq(Process processes[], int n, Metrics *metrics);
void calculate_metrics(Process processes[], int n, int total_time, Metrics *metrics);
void print_results(Process processes[], int n, Metrics metrics, const char *algorithm);
void save_to_file(Process processes[], int n, Metrics metrics, const char *algorithm);
void run_comprehensive_tests();
void display_gantt_chart(int pid_sequence[], int time_sequence[], int sequence_length);
void sort_by_arrival(Process processes[], int n);
void reset_processes(Process original[], Process copy[], int n);

// Main function
int main() {
    Process processes[MAX_PROCESSES];
    int n = 0, choice, quantum;

    printf("\n");
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║                                                            ║\n");
    printf("║        CPU SCHEDULING SIMULATOR - OwlTech Industries       ║\n");
    printf("║                                                            ║\n");
    printf("║              Performance Optimization Division             ║\n");
    printf("║                                                            ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
    printf("\n");

    while (1) {
        display_menu();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        Metrics metrics = {0};

        switch (choice) {
            case 1:
                input_processes(processes, &n);
                break;

            case 2:
                printf("\nSelect workload type:\n");
                printf("1. CPU-bound (long burst times)\n");
                printf("2. I/O-bound (short burst times)\n");
                printf("3. Mixed workload\n");
                printf("Choice: ");
                int type;
                scanf("%d", &type);
                generate_test_workload(processes, &n, type);
                printf("✓ Generated %d processes\n", n);
                break;

            case 3:
                if (n == 0) {
                    printf("⚠ No processes loaded! Please add processes first.\n");
                    break;
                }
                fcfs(processes, n, &metrics);
                print_results(processes, n, metrics, "FCFS");
                save_to_file(processes, n, metrics, "FCFS");
                break;

            case 4:
                if (n == 0) {
                    printf("⚠ No processes loaded! Please add processes first.\n");
                    break;
                }
                sjf(processes, n, &metrics);
                print_results(processes, n, metrics, "SJF");
                save_to_file(processes, n, metrics, "SJF");
                break;

            case 5:
                if (n == 0) {
                    printf("⚠ No processes loaded! Please add processes first.\n");
                    break;
                }
                priority_scheduling(processes, n, &metrics);
                print_results(processes, n, metrics, "Priority");
                save_to_file(processes, n, metrics, "Priority");
                break;

            case 6:
                if (n == 0) {
                    printf("⚠ No processes loaded! Please add processes first.\n");
                    break;
                }
                printf("Enter time quantum: ");
                scanf("%d", &quantum);
                round_robin(processes, n, quantum, &metrics);
                print_results(processes, n, metrics, "Round Robin");
                save_to_file(processes, n, metrics, "Round Robin");
                break;

            case 7:
                if (n == 0) {
                    printf("⚠ No processes loaded! Please add processes first.\n");
                    break;
                }
                srtf(processes, n, &metrics);
                print_results(processes, n, metrics, "SRTF");
                save_to_file(processes, n, metrics, "SRTF");
                break;

            case 8:
                if (n == 0) {
                    printf("⚠ No processes loaded! Please add processes first.\n");
                    break;
                }
                mlfq(processes, n, &metrics);
                print_results(processes, n, metrics, "MLFQ");
                save_to_file(processes, n, metrics, "MLFQ");
                break;

            case 9:
                run_comprehensive_tests();
                break;

            case 0:
                printf("\n✓ Thank you for using OwlTech CPU Scheduler!\n");
                printf("  Results saved to 'scheduling_results.txt'\n\n");
                return 0;

            default:
                printf("⚠ Invalid choice! Please try again.\n");
        }

        printf("\nPress Enter to continue...");
        getchar(); getchar();
    }

    return 0;
}

void display_menu() {
    printf("\n");
    printf("┌────────────────────────────────────────────────────────────┐\n");
    printf("│                        MAIN MENU                           │\n");
    printf("├────────────────────────────────────────────────────────────┤\n");
    printf("│  Process Input:                                            │\n");
    printf("│    1. Manual Input                                         │\n");
    printf("│    2. Generate Test Workload                               │\n");
    printf("│                                                            │\n");
    printf("│  Existing Algorithms:                                      │\n");
    printf("│    3. First Come First Serve (FCFS)                       │\n");
    printf("│    4. Shortest Job First (SJF)                            │\n");
    printf("│    5. Priority Scheduling                                 │\n");
    printf("│    6. Round Robin (RR)                                    │\n");
    printf("│                                                            │\n");
    printf("│  New Algorithms:                                           │\n");
    printf("│    7. Shortest Remaining Time First (SRTF) ⭐             │\n");
    printf("│    8. Multi-Level Feedback Queue (MLFQ) ⭐                │\n");
    printf("│                                                            │\n");
    printf("│  Testing:                                                  │\n");
    printf("│    9. Run Comprehensive Tests                              │\n");
    printf("│                                                            │\n");
    printf("│    0. Exit                                                 │\n");
    printf("└────────────────────────────────────────────────────────────┘\n");
}

void input_processes(Process processes[], int *n) {
    printf("\nEnter number of processes (1-%d): ", MAX_PROCESSES);
    scanf("%d", n);

    if (*n <= 0 || *n > MAX_PROCESSES) {
        printf("⚠ Invalid number of processes!\n");
        *n = 0;
        return;
    }

    for (int i = 0; i < *n; i++) {
        printf("\n--- Process P%d ---\n", i + 1);
        processes[i].pid = i + 1;

        printf("Arrival Time: ");
        scanf("%d", &processes[i].arrival_time);

        printf("Burst Time: ");
        scanf("%d", &processes[i].burst_time);

        printf("Priority (lower number = higher priority): ");
        scanf("%d", &processes[i].priority);

        processes[i].remaining_time = processes[i].burst_time;
        processes[i].waiting_time = 0;
        processes[i].turnaround_time = 0;
        processes[i].completion_time = 0;
        processes[i].response_time = -1;
        processes[i].first_response = false;
        processes[i].queue_level = 0;
    }

    printf("\n✓ Successfully loaded %d processes\n", *n);
}

void generate_test_workload(Process processes[], int *n, int type) {
    srand(time(NULL));

    printf("Enter number of processes to generate (5-100): ");
    scanf("%d", n);

    if (*n < 5 || *n > MAX_PROCESSES) {
        printf("⚠ Invalid number! Setting to 10.\n");
        *n = 10;
    }

    for (int i = 0; i < *n; i++) {
        processes[i].pid = i + 1;
        processes[i].arrival_time = rand() % 20;

        switch (type) {
            case 1: // CPU-bound
                processes[i].burst_time = 20 + rand() % 80;
                break;
            case 2: // I/O-bound
                processes[i].burst_time = 1 + rand() % 10;
                break;
            case 3: // Mixed
                if (rand() % 2)
                    processes[i].burst_time = 1 + rand() % 15;
                else
                    processes[i].burst_time = 20 + rand() % 50;
                break;
            default:
                processes[i].burst_time = 5 + rand() % 20;
        }

        processes[i].priority = 1 + rand() % 10;
        processes[i].remaining_time = processes[i].burst_time;
        processes[i].waiting_time = 0;
        processes[i].turnaround_time = 0;
        processes[i].completion_time = 0;
        processes[i].response_time = -1;
        processes[i].first_response = false;
        processes[i].queue_level = 0;
    }
}

void sort_by_arrival(Process processes[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (processes[j].arrival_time > processes[j + 1].arrival_time) {
                Process temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
        }
    }
}

void reset_processes(Process original[], Process copy[], int n) {
    for (int i = 0; i < n; i++) {
        copy[i] = original[i];
        copy[i].remaining_time = copy[i].burst_time;
        copy[i].waiting_time = 0;
        copy[i].turnaround_time = 0;
        copy[i].completion_time = 0;
        copy[i].response_time = -1;
        copy[i].first_response = false;
        copy[i].queue_level = 0;
    }
}

// FCFS Algorithm
void fcfs(Process processes[], int n, Metrics *metrics) {
    Process proc[MAX_PROCESSES];
    reset_processes(processes, proc, n);
    sort_by_arrival(proc, n);

    int current_time = 0;

    for (int i = 0; i < n; i++) {
        if (current_time < proc[i].arrival_time) {
            current_time = proc[i].arrival_time;
        }

        proc[i].response_time = current_time - proc[i].arrival_time;
        current_time += proc[i].burst_time;
        proc[i].completion_time = current_time;
        proc[i].turnaround_time = proc[i].completion_time - proc[i].arrival_time;
        proc[i].waiting_time = proc[i].turnaround_time - proc[i].burst_time;
    }

    calculate_metrics(proc, n, current_time, metrics);

    for (int i = 0; i < n; i++) {
        processes[i] = proc[i];
    }
}

// SJF Algorithm
void sjf(Process processes[], int n, Metrics *metrics) {
    Process proc[MAX_PROCESSES];
    reset_processes(processes, proc, n);

    int current_time = 0, completed = 0;
    bool visited[MAX_PROCESSES] = {false};

    while (completed < n) {
        int idx = -1;
        int min_burst = 999999;

        for (int i = 0; i < n; i++) {
            if (proc[i].arrival_time <= current_time && !visited[i]) {
                if (proc[i].burst_time < min_burst) {
                    min_burst = proc[i].burst_time;
                    idx = i;
                }
            }
        }

        if (idx != -1) {
            proc[idx].response_time = current_time - proc[idx].arrival_time;
            current_time += proc[idx].burst_time;
            proc[idx].completion_time = current_time;
            proc[idx].turnaround_time = proc[idx].completion_time - proc[idx].arrival_time;
            proc[idx].waiting_time = proc[idx].turnaround_time - proc[idx].burst_time;
            visited[idx] = true;
            completed++;
        } else {
            current_time++;
        }
    }

    calculate_metrics(proc, n, current_time, metrics);

    for (int i = 0; i < n; i++) {
        processes[i] = proc[i];
    }
}

// Priority Scheduling
void priority_scheduling(Process processes[], int n, Metrics *metrics) {
    Process proc[MAX_PROCESSES];
    reset_processes(processes, proc, n);

    int current_time = 0, completed = 0;
    bool visited[MAX_PROCESSES] = {false};

    while (completed < n) {
        int idx = -1;
        int highest_priority = 999999;

        for (int i = 0; i < n; i++) {
            if (proc[i].arrival_time <= current_time && !visited[i]) {
                if (proc[i].priority < highest_priority) {
                    highest_priority = proc[i].priority;
                    idx = i;
                }
            }
        }

        if (idx != -1) {
            proc[idx].response_time = current_time - proc[idx].arrival_time;
            current_time += proc[idx].burst_time;
            proc[idx].completion_time = current_time;
            proc[idx].turnaround_time = proc[idx].completion_time - proc[idx].arrival_time;
            proc[idx].waiting_time = proc[idx].turnaround_time - proc[idx].burst_time;
            visited[idx] = true;
            completed++;
        } else {
            current_time++;
        }
    }

    calculate_metrics(proc, n, current_time, metrics);

    for (int i = 0; i < n; i++) {
        processes[i] = proc[i];
    }
}

// Round Robin Algorithm
void round_robin(Process processes[], int n, int quantum, Metrics *metrics) {
    Process proc[MAX_PROCESSES];
    reset_processes(processes, proc, n);

    int current_time = 0, completed = 0;
    int queue[MAX_PROCESSES * 10];
    int front = 0, rear = 0;
    bool in_queue[MAX_PROCESSES] = {false};

    // Add first arriving process
    int min_arrival = 999999;
    int first_idx = 0;
    for (int i = 0; i < n; i++) {
        if (proc[i].arrival_time < min_arrival) {
            min_arrival = proc[i].arrival_time;
            first_idx = i;
        }
    }

    current_time = proc[first_idx].arrival_time;
    queue[rear++] = first_idx;
    in_queue[first_idx] = true;

    while (completed < n) {
        if (front == rear) {
            // Queue empty, find next arriving process
            int next_arrival = 999999;
            int next_idx = -1;
            for (int i = 0; i < n; i++) {
                if (proc[i].remaining_time > 0 && proc[i].arrival_time > current_time) {
                    if (proc[i].arrival_time < next_arrival) {
                        next_arrival = proc[i].arrival_time;
                        next_idx = i;
                    }
                }
            }
            if (next_idx != -1) {
                current_time = proc[next_idx].arrival_time;
                queue[rear++] = next_idx;
                in_queue[next_idx] = true;
            }
            continue;
        }

        int idx = queue[front++];

        if (proc[idx].remaining_time > 0) {
            if (!proc[idx].first_response) {
                proc[idx].response_time = current_time - proc[idx].arrival_time;
                proc[idx].first_response = true;
            }

            int exec_time = (proc[idx].remaining_time > quantum) ? quantum : proc[idx].remaining_time;
            proc[idx].remaining_time -= exec_time;
            current_time += exec_time;

            // Add newly arrived processes
            for (int i = 0; i < n; i++) {
                if (proc[i].arrival_time <= current_time && proc[i].remaining_time > 0 && !in_queue[i] && i != idx) {
                    queue[rear++] = i;
                    in_queue[i] = true;
                }
            }

            if (proc[idx].remaining_time == 0) {
                proc[idx].completion_time = current_time;
                proc[idx].turnaround_time = proc[idx].completion_time - proc[idx].arrival_time;
                proc[idx].waiting_time = proc[idx].turnaround_time - proc[idx].burst_time;
                completed++;
                in_queue[idx] = false;
            } else {
                queue[rear++] = idx;
            }
        }
    }

    calculate_metrics(proc, n, current_time, metrics);

    for (int i = 0; i < n; i++) {
        processes[i] = proc[i];
    }
}

// SRTF Algorithm (Shortest Remaining Time First) - NEW!
void srtf(Process processes[], int n, Metrics *metrics) {
    Process proc[MAX_PROCESSES];
    reset_processes(processes, proc, n);

    int current_time = 0, completed = 0;
    int total_burst = 0;

    for (int i = 0; i < n; i++) {
        total_burst += proc[i].burst_time;
    }

    while (completed < n) {
        int idx = -1;
        int min_remaining = 999999;

        // Find process with shortest remaining time
        for (int i = 0; i < n; i++) {
            if (proc[i].arrival_time <= current_time && proc[i].remaining_time > 0) {
                if (proc[i].remaining_time < min_remaining) {
                    min_remaining = proc[i].remaining_time;
                    idx = i;
                }
            }
        }

        if (idx != -1) {
            // Record response time on first execution
            if (!proc[idx].first_response) {
                proc[idx].response_time = current_time - proc[idx].arrival_time;
                proc[idx].first_response = true;
            }

            proc[idx].remaining_time--;
            current_time++;

            if (proc[idx].remaining_time == 0) {
                proc[idx].completion_time = current_time;
                proc[idx].turnaround_time = proc[idx].completion_time - proc[idx].arrival_time;
                proc[idx].waiting_time = proc[idx].turnaround_time - proc[idx].burst_time;
                completed++;
            }
        } else {
            current_time++;
        }
    }

    calculate_metrics(proc, n, current_time, metrics);

    for (int i = 0; i < n; i++) {
        processes[i] = proc[i];
    }
}

// MLFQ Algorithm (Multi-Level Feedback Queue) - NEW!
void mlfq(Process processes[], int n, Metrics *metrics) {
    Process proc[MAX_PROCESSES];
    reset_processes(processes, proc, n);

    int quantum[MAX_QUEUES] = {2, 4, 8, 16, 32};
    int current_time = 0, completed = 0;

    typedef struct {
        int pid_idx;
        int queue_level;
    } QueueItem;

    QueueItem queue[MAX_PROCESSES * MAX_QUEUES];
    int front = 0, rear = 0;
    bool in_queue[MAX_PROCESSES] = {false};

    // Initialize all processes to queue level 0
    for (int i = 0; i < n; i++) {
        proc[i].queue_level = 0;
    }

    // Add first arriving process
    int min_arrival = 999999;
    int first_idx = 0;
    for (int i = 0; i < n; i++) {
        if (proc[i].arrival_time < min_arrival) {
            min_arrival = proc[i].arrival_time;
            first_idx = i;
        }
    }

    current_time = proc[first_idx].arrival_time;
    queue[rear].pid_idx = first_idx;
    queue[rear].queue_level = 0;
    rear++;
    in_queue[first_idx] = true;

    while (completed < n) {
        if (front == rear) {
            int next_arrival = 999999;
            int next_idx = -1;
            for (int i = 0; i < n; i++) {
                if (proc[i].remaining_time > 0 && proc[i].arrival_time > current_time) {
                    if (proc[i].arrival_time < next_arrival) {
                        next_arrival = proc[i].arrival_time;
                        next_idx = i;
                    }
                }
            }
            if (next_idx != -1) {
                current_time = proc[next_idx].arrival_time;
                queue[rear].pid_idx = next_idx;
                queue[rear].queue_level = proc[next_idx].queue_level;
                rear++;
                in_queue[next_idx] = true;
            }
            continue;
        }

        // Find highest priority (lowest queue level) process
        int selected_idx = front;
        for (int i = front; i < rear; i++) {
            if (queue[i].queue_level < queue[selected_idx].queue_level) {
                selected_idx = i;
            }
        }

        QueueItem current = queue[selected_idx];
        // Remove from queue by shifting
        for (int i = selected_idx; i < rear - 1; i++) {
            queue[i] = queue[i + 1];
        }
        rear--;

        int idx = current.pid_idx;
        int level = proc[idx].queue_level;

        if (proc[idx].remaining_time > 0) {
            if (!proc[idx].first_response) {
                proc[idx].response_time = current_time - proc[idx].arrival_time;
                proc[idx].first_response = true;
            }

            int time_quantum = quantum[level];
            int exec_time = (proc[idx].remaining_time > time_quantum) ? time_quantum : proc[idx].remaining_time;
            proc[idx].remaining_time -= exec_time;
            current_time += exec_time;

            // Add newly arrived processes
            for (int i = 0; i < n; i++) {
                if (proc[i].arrival_time <= current_time && proc[i].remaining_time > 0 && !in_queue[i]) {
                    queue[rear].pid_idx = i;
                    queue[rear].queue_level = proc[i].queue_level;
                    rear++;
                    in_queue[i] = true;
                }
            }

            if (proc[idx].remaining_time == 0) {
                proc[idx].completion_time = current_time;
                proc[idx].turnaround_time = proc[idx].completion_time - proc[idx].arrival_time;
                proc[idx].waiting_time = proc[idx].turnaround_time - proc[idx].burst_time;
                completed++;
                in_queue[idx] = false;
            } else {
                // Move to lower priority queue if not completed
                if (proc[idx].queue_level < MAX_QUEUES - 1) {
                    proc[idx].queue_level++;
                }
                queue[rear].pid_idx = idx;
                queue[rear].queue_level = proc[idx].queue_level;
                rear++;
            }
        }
    }

    calculate_metrics(proc, n, current_time, metrics);

    for (int i = 0; i < n; i++) {
        processes[i] = proc[i];
    }
}

void calculate_metrics(Process processes[], int n, int total_time, Metrics *metrics) {
    double total_waiting = 0, total_turnaround = 0, total_response = 0;
    int total_burst = 0;

    for (int i = 0; i < n; i++) {
        total_waiting += processes[i].waiting_time;
        total_turnaround += processes[i].turnaround_time;
        total_response += processes[i].response_time;
        total_burst += processes[i].burst_time;
    }

    metrics->avg_waiting_time = total_waiting / n;
    metrics->avg_turnaround_time = total_turnaround / n;
    metrics->avg_response_time = total_response / n;
    metrics->cpu_utilization = (total_burst / (double)total_time) * 100.0;
    metrics->throughput = n / (double)total_time;
    metrics->total_time = total_time;
}

void print_results(Process processes[], int n, Metrics metrics, const char *algorithm) {
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║              %s SCHEDULING RESULTS                    \n", algorithm);
    printf("╚════════════════════════════════════════════════════════════╝\n\n");

    printf("┌─────┬─────────┬──────────┬──────────┬────────────┬───────────┬──────────┐\n");
    printf("│ PID │ Arrival │ Burst    │ Priority │ Waiting    │ Turnaround│ Response │\n");
    printf("├─────┼─────────┼──────────┼──────────┼────────────┼───────────┼──────────┤\n");

    for (int i = 0; i < n; i++) {
        printf("│ P%-3d│   %-6d│   %-7d│    %-6d│    %-8d│    %-7d│   %-7d│\n",
               processes[i].pid,
               processes[i].arrival_time,
               processes[i].burst_time,
               processes[i].priority,
               processes[i].waiting_time,
               processes[i].turnaround_time,
               processes[i].response_time);
    }

    printf("└─────┴─────────┴──────────┴──────────┴────────────┴───────────┴──────────┘\n\n");

    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║                   PERFORMANCE METRICS                      ║\n");
    printf("╠════════════════════════════════════════════════════════════╣\n");
    printf("║  Average Waiting Time:        %.2f time units          \n", metrics.avg_waiting_time);
    printf("║  Average Turnaround Time:     %.2f time units          \n", metrics.avg_turnaround_time);
    printf("║  Average Response Time:       %.2f time units          \n", metrics.avg_response_time);
    printf("║  CPU Utilization:             %.2f%%                   \n", metrics.cpu_utilization);
    printf("║  Throughput:                  %.4f processes/unit      \n", metrics.throughput);
    printf("║  Total Execution Time:        %d time units            \n", metrics.total_time);
    printf("╚════════════════════════════════════════════════════════════╝\n");
}

void save_to_file(Process processes[], int n, Metrics metrics, const char *algorithm) {
    FILE *fp = fopen("scheduling_results.txt", "a");
    if (fp == NULL) {
        printf("⚠ Error opening file for writing!\n");
        return;
    }

    time_t now;
    time(&now);

    fprintf(fp, "\n========================================\n");
    fprintf(fp, "Algorithm: %s\n", algorithm);
    fprintf(fp, "Timestamp: %s", ctime(&now));
    fprintf(fp, "Number of Processes: %d\n", n);
    fprintf(fp, "========================================\n\n");

    fprintf(fp, "Process Details:\n");
    fprintf(fp, "PID\tArrival\tBurst\tPriority\tWaiting\tTurnaround\tResponse\n");
    fprintf(fp, "--------------------------------------------------------------------\n");

    for (int i = 0; i < n; i++) {
        fprintf(fp, "P%d\t%d\t%d\t%d\t\t%d\t%d\t\t%d\n",
                processes[i].pid,
                processes[i].arrival_time,
                processes[i].burst_time,
                processes[i].priority,
                processes[i].waiting_time,
                processes[i].turnaround_time,
                processes[i].response_time);
    }

    fprintf(fp, "\nPerformance Metrics:\n");
    fprintf(fp, "--------------------------------------------------------------------\n");
    fprintf(fp, "Average Waiting Time:      %.2f\n", metrics.avg_waiting_time);
    fprintf(fp, "Average Turnaround Time:   %.2f\n", metrics.avg_turnaround_time);
    fprintf(fp, "Average Response Time:     %.2f\n", metrics.avg_response_time);
    fprintf(fp, "CPU Utilization:           %.2f%%\n", metrics.cpu_utilization);
    fprintf(fp, "Throughput:                %.4f processes/unit\n", metrics.throughput);
    fprintf(fp, "Total Execution Time:      %d time units\n", metrics.total_time);
    fprintf(fp, "\n\n");

    fclose(fp);
}

void run_comprehensive_tests() {
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║              COMPREHENSIVE TEST SUITE                      ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");

    // Clear previous results
    FILE *fp = fopen("scheduling_results.txt", "w");
    if (fp != NULL) {
        fprintf(fp, "CPU SCHEDULING SIMULATOR - COMPREHENSIVE TEST RESULTS\n");
        fprintf(fp, "OwlTech Industries - Performance Optimization Division\n");
        fprintf(fp, "======================================================\n\n");
        fclose(fp);
    }

    // Open comparison file
    FILE *comp_fp = fopen("algorithm_comparison.csv", "w");
    if (comp_fp != NULL) {
        fprintf(comp_fp, "Test Name,Algorithm,Avg Waiting,Avg Turnaround,Avg Response,CPU Util,Throughput\n");
    }

    Process test_processes[MAX_PROCESSES];
    int test_sizes[] = {5, 10, 20, 50};
    char *test_names[] = {"Small", "Medium", "Large", "XLarge"};
    int num_tests = 4;

    for (int test = 0; test < num_tests; test++) {
        for (int workload = 1; workload <= 3; workload++) {
            int n = test_sizes[test];

            printf("\n┌────────────────────────────────────────────────────────┐\n");
            printf("│ Test: %s Set - ", test_names[test]);

            switch (workload) {
                case 1:
                    printf("CPU-Bound Workload                  │\n");
                    break;
                case 2:
                    printf("I/O-Bound Workload                  │\n");
                    break;
                case 3:
                    printf("Mixed Workload                      │\n");
                    break;
            }
            printf("└────────────────────────────────────────────────────────┘\n");

            generate_test_workload(test_processes, &n, workload);

            // Save test data
            FILE *test_fp = fopen("scheduling_results.txt", "a");
            if (test_fp != NULL) {
                fprintf(test_fp, "\n>>> TEST: %s Set - ", test_names[test]);
                switch (workload) {
                    case 1: fprintf(test_fp, "CPU-Bound\n"); break;
                    case 2: fprintf(test_fp, "I/O-Bound\n"); break;
                    case 3: fprintf(test_fp, "Mixed\n"); break;
                }
                fclose(test_fp);
            }

            // Test all algorithms
            Metrics metrics;
            Process proc_copy[MAX_PROCESSES];

            // FCFS
            printf("  Testing FCFS... ");
            reset_processes(test_processes, proc_copy, n);
            fcfs(proc_copy, n, &metrics);
            save_to_file(proc_copy, n, metrics, "FCFS");
            if (comp_fp != NULL) {
                fprintf(comp_fp, "%s-%d,FCFS,%.2f,%.2f,%.2f,%.2f,%.4f\n",
                        test_names[test], workload, metrics.avg_waiting_time,
                        metrics.avg_turnaround_time, metrics.avg_response_time,
                        metrics.cpu_utilization, metrics.throughput);
            }
            printf("✓\n");

            // SJF
            printf("  Testing SJF... ");
            reset_processes(test_processes, proc_copy, n);
            sjf(proc_copy, n, &metrics);
            save_to_file(proc_copy, n, metrics, "SJF");
            if (comp_fp != NULL) {
                fprintf(comp_fp, "%s-%d,SJF,%.2f,%.2f,%.2f,%.2f,%.4f\n",
                        test_names[test], workload, metrics.avg_waiting_time,
                        metrics.avg_turnaround_time, metrics.avg_response_time,
                        metrics.cpu_utilization, metrics.throughput);
            }
            printf("✓\n");

            // Priority
            printf("  Testing Priority... ");
            reset_processes(test_processes, proc_copy, n);
            priority_scheduling(proc_copy, n, &metrics);
            save_to_file(proc_copy, n, metrics, "Priority");
            if (comp_fp != NULL) {
                fprintf(comp_fp, "%s-%d,Priority,%.2f,%.2f,%.2f,%.2f,%.4f\n",
                        test_names[test], workload, metrics.avg_waiting_time,
                        metrics.avg_turnaround_time, metrics.avg_response_time,
                        metrics.cpu_utilization, metrics.throughput);
            }
            printf("✓\n");

            // Round Robin
            printf("  Testing Round Robin... ");
            reset_processes(test_processes, proc_copy, n);
            round_robin(proc_copy, n, 4, &metrics);
            save_to_file(proc_copy, n, metrics, "Round Robin (q=4)");
            if (comp_fp != NULL) {
                fprintf(comp_fp, "%s-%d,RR(q=4),%.2f,%.2f,%.2f,%.2f,%.4f\n",
                        test_names[test], workload, metrics.avg_waiting_time,
                        metrics.avg_turnaround_time, metrics.avg_response_time,
                        metrics.cpu_utilization, metrics.throughput);
            }
            printf("✓\n");

            // SRTF
            printf("  Testing SRTF... ");
            reset_processes(test_processes, proc_copy, n);
            srtf(proc_copy, n, &metrics);
            save_to_file(proc_copy, n, metrics, "SRTF");
            if (comp_fp != NULL) {
                fprintf(comp_fp, "%s-%d,SRTF,%.2f,%.2f,%.2f,%.2f,%.4f\n",
                        test_names[test], workload, metrics.avg_waiting_time,
                        metrics.avg_turnaround_time, metrics.avg_response_time,
                        metrics.cpu_utilization, metrics.throughput);
            }
            printf("✓\n");

            // MLFQ
            printf("  Testing MLFQ... ");
            reset_processes(test_processes, proc_copy, n);
            mlfq(proc_copy, n, &metrics);
            save_to_file(proc_copy, n, metrics, "MLFQ");
            if (comp_fp != NULL) {
                fprintf(comp_fp, "%s-%d,MLFQ,%.2f,%.2f,%.2f,%.2f,%.4f\n",
                        test_names[test], workload, metrics.avg_waiting_time,
                        metrics.avg_turnaround_time, metrics.avg_response_time,
                        metrics.cpu_utilization, metrics.throughput);
            }
            printf("✓\n");
        }
    }

    if (comp_fp != NULL) {
        fclose(comp_fp);
    }

    printf("\n");
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║            COMPREHENSIVE TESTS COMPLETED!                  ║\n");
    printf("╠════════════════════════════════════════════════════════════╣\n");
    printf("║  Results saved to:                                         ║\n");
    printf("║    • scheduling_results.txt (Detailed results)             ║\n");
    printf("║    • algorithm_comparison.csv (Comparison table)           ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
}