# CPU Scheduling Simulator

**CS 3502: Operating Systems - Project 2**  
**OwlTech Industries - Performance Optimization Division**

---

## 📋 Project Overview

This CPU Scheduling Simulator implements and compares six different CPU scheduling algorithms:

### Existing Algorithms (4)
1. **FCFS** (First Come First Serve)
2. **SJF** (Shortest Job First)
3. **Priority Scheduling**
4. **Round Robin**

### New Implementations (2) ⭐
5. **SRTF** (Shortest Remaining Time First)
6. **MLFQ** (Multi-Level Feedback Queue)

---

## 🚀 Quick Start

### Prerequisites
- GCC compiler
- Linux/Unix environment (or Windows with MinGW)

### Build & Run
```bash
# Compile the program
make

# Run the simulator
make run

# Run comprehensive tests
make test
```

### Manual Compilation
```bash
gcc -Wall -Wextra -std=c11 -O2 -o cpu_scheduler cpu_scheduler.c
./cpu_scheduler
```

---

## 📊 Features

### Performance Metrics Calculated
- ✓ **Average Waiting Time (AWT)**
- ✓ **Average Turnaround Time (ATT)**
- ✓ **Average Response Time**
- ✓ **CPU Utilization (%)**
- ✓ **Throughput (processes/time unit)**

### Test Workloads
1. **CPU-bound**: Long burst times (20-100 units)
2. **I/O-bound**: Short burst times (1-10 units)
3. **Mixed**: Combination of both

### Test Sizes Used
- **Small**: 5 processes
- **Medium**: 20 processes
- **Large**: 40 processes
- **X-Large**: 60 processes

**Total Tests Conducted**: 4 sizes × 3 workloads × 6 algorithms = **72 comprehensive test runs**

### Output Files
- `scheduling_results.txt` - Detailed results for each algorithm
- `algorithm_comparison.csv` - Comparison table 

---

## 🎮 User Interface

The simulator provides an interactive menu:

```
┌────────────────────────────────────────────────────────────┐
│                        MAIN MENU                           │
├────────────────────────────────────────────────────────────┤
│  Process Input:                                            │
│    1. Manual Input                                         │
│    2. Generate Test Workload                               │
│                                                            │
│  Existing Algorithms:                                      │
│    3. First Come First Serve (FCFS)                       │
│    4. Shortest Job First (SJF)                            │
│    5. Priority Scheduling                                 │
│    6. Round Robin (RR)                                    │
│                                                            │
│  New Algorithms:                                           │
│    7. Shortest Remaining Time First (SRTF) ⭐             │
│    8. Multi-Level Feedback Queue (MLFQ) ⭐                │
│                                                            │
│  Testing:                                                  │
│    9. Run Comprehensive Tests                              │
│                                                            │
│    0. Exit                                                 │
└────────────────────────────────────────────────────────────┘
```

---

## 🔬 Algorithm Implementations

### 1. SRTF (Shortest Remaining Time First)
**Type**: Preemptive  
**Description**: Preemptive version of SJF. At each time unit, the process with the shortest remaining burst time is executed.

**Key Features**:
- Optimal average waiting time
- May cause starvation for long processes
- High context switching overhead

**Use Case**: Systems prioritizing short jobs with acceptable context switching costs

### 2. MLFQ (Multi-Level Feedback Queue)
**Type**: Preemptive, Adaptive  
**Description**: Uses multiple queues with different priorities and time quantums. Processes start at highest priority and move down if they use their full quantum.

**Queue Configuration**:
- Queue 0: Time quantum = 2 (highest priority)
- Queue 1: Time quantum = 4
- Queue 2: Time quantum = 8
- Queue 3: Time quantum = 16
- Queue 4: Time quantum = 32 (lowest priority)

**Key Features**:
- Balances responsiveness and fairness
- Adapts to process behavior
- Favors I/O-bound processes

**Use Case**: General-purpose systems with mixed workloads (similar to modern OS schedulers)

---

## 📈 Testing Strategy

### Comprehensive Test Suite

The simulator includes automated testing with:

#### Test Sizes
- Small (5 processes)
- Medium (20 processes)
- Large (40 processes)
- XLarge (60 processes)

#### Workload Types (for each size)
1. CPU-bound
2. I/O-bound
3. Mixed

**Total Tests**: 4 sizes × 3 workloads × 6 algorithms = **72 test runs**

---

## 📁 Project Structure

```
cpu-scheduler/
├── cpu_scheduler.c          # Main source code
├── Makefile                 # Build automation
├── README.md               # This file
├── scheduling_results.txt   # Generated: Detailed results
├── algorithm_comparison.csv # Generated: Comparison data
└── report/                 
    ├── report.tex
    ├── figures/
    └── tables/
```

---

## 📝 Example Usage

### Manual Process Input
```bash
$ ./cpu_scheduler
# Select option 1
# Enter process details when prompted
# Select algorithm (3-8)
```

### Generate & Test
```bash
$ ./cpu_scheduler
# Select option 2 (Generate Test Workload)
# Choose workload type (1-3)
# Enter number of processes
# Select algorithm to test
```

### Run All Tests
```bash
$ ./cpu_scheduler
# Select option 9
# Wait for completion
# Check scheduling_results.txt and algorithm_comparison.csv
```

---

## 📊 Output Format

### Console Output
```
╔════════════════════════════════════════════════════════════╗
║              SRTF SCHEDULING RESULTS                       ║
╚════════════════════════════════════════════════════════════╝

┌─────┬─────────┬──────────┬──────────┬────────────┬───────────┬──────────┐
│ PID │ Arrival │ Burst    │ Priority │ Waiting    │ Turnaround│ Response │
├─────┼─────────┼──────────┼──────────┼────────────┼───────────┼──────────┤
│ P1  │   0     │   8      │    3     │    0       │    8      │   0      │
│ P2  │   1     │   4      │    1     │    0       │    3      │   0      │
...
```

### File Output (CSV)
```csv
Test Name,Algorithm,Avg Waiting,Avg Turnaround,Avg Response,CPU Util,Throughput
Small-1,FCFS,15.20,23.40,15.20,95.23,0.0423
Small-1,SJF,10.80,18.80,10.80,95.23,0.0423
Small-1,SRTF,8.40,16.40,5.60,95.23,0.0423
...
```



## 🏆 Recommendations for OwlTech

Based on comprehensive testing, include recommendations such as:

1. **For general-purpose systems**: MLFQ
   - Balances fairness and responsiveness
   - Adapts to workload characteristics

2. **For batch processing**: SJF
   - Minimizes average waiting time
   - No preemption overhead

3. **For real-time systems**: Priority Scheduling
   - Guarantees critical task execution
   - Predictable behavior

---

## 🐛 Known Limitations

1. No I/O simulation (pure CPU scheduling)
2. Fixed MLFQ queue configuration
3. No process aging to prevent starvation
4. Integer time units only

---

## 📚 References

- Operating System Concepts (Silberschatz, Galvin, Gagne)
- [UIC OS Notes - CPU Scheduling](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/5_CPU_Scheduling.html)

---

## 👨‍💻 Author

**Zakaria Sherif**  
CS 3502 - Operating Systems  
Kennesaw State University  
Fall 2024

---

## 📜 License

This project is for educational purposes as part of CS 3502 coursework.

---

