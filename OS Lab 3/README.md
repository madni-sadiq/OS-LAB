# Scheduling Algorithms 
This project involves implementing several different process scheduling algorithms. The scheduler will be assigned a predefined set of tasks and will schedule the tasks based on the selected scheduling algorithm. Each task is assigned a priority and CPU burst. The following scheduling algorithms will be implemented: 
* First-come, first-served (FCFS), which schedules tasks in the order in which they request the CPU. 
* Shortest-job-first (SJF), which schedules tasks in order of the length of the tasks’ next CPU burst. 
* Priority scheduling, which schedules tasks based on priority. 
* Round-robin (RR) scheduling, where each task is run for a time quantum (or for the remainder of its CPU burst). 
* Priority with round-robin, also called Multilevel Queue Scheduling (MQS), which schedules tasks in order of priority and uses round-robin scheduling for tasks with equal priority.
* Multilevel feedback Queue, which has three Queues of time Quantum 10 ms, 20 ms and 30 ms from first to last queue respectively. Only demotion of a task is implemented (not promotion).
Priorities range from 1 to 10, where a higher numeric value indicates a higher relative priority. For round-robin scheduling, the length of a time quantum is 10 milliseconds.
## Implementation
Filename of each algorithm file is of the form **"schedule_\<algorithm name\>.c"**.
E.g., 
1. Algorithm of _fcfs_ is implemented in _schedule_fcfs.c_
2. Algorithm of _sjf_ is implemented in _schedule_sjfs.c_
3. Algorithm of _priority scheduling_ is implemented in _schedule_priority.c_
4. Algorithm of _rr_ is implemented in _schedule_rr.c_
5. Algorithm of _mqs_ is implemented in _schedule_mqs.c_
6. Algorithm of _mfq_ is implemented in _schedule_mfq.c_
## Compilation & Execution
There is added a Makefile in repository which will build the necessary files to run an algorithm. Command to build the desired algorithm is *`make <algorithm name>`*. \
E.g., in order to build the algorithm of fcfs, type: `make fcfs`\
Similarly, to build the algorithm of sjf, type: `make sjf`, and so on.\
After building codes, they can be executed by typing name of the algorithm followed by the filename containing tasks with their priority number and burst time.\
E.g., if fcfs algorithm has been built, it can be executed by using the command: `./fcfs schedule.txt` \
where, schedule.txt is name of the file containing tasks and their priority number and burst time.
