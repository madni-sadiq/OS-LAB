#define MIN_PRIORITY 1
#define MAX_PRIORITY 10
#define time_quantum 10
#include "task.h"
#include "list.h"
#include "cpu.h"
#include<stdlib.h>
#include<stdio.h>
int tid = 0;
// add a task to the list
void add(char *name, int priority, int burst, struct node **L){
	Task* t;
	t = malloc(sizeof(struct task));
	t->tid = tid+1;
	t->name = name;
	t->priority = priority;
	t->burst = burst;
	insert(L, t);
}

// invoke the scheduler
void schedule(struct node *head){
	// creating a copy of head list
	struct node **copyL = malloc(sizeof(struct node));
	*copyL = (struct node*)malloc(sizeof(struct node));
	struct node* tmpNode;
	struct node *temp;
	(*copyL)->next = NULL;
	head = head->next;
	temp = head;
	// adding tasks in copy list
	while (head != NULL){
		Task *t = malloc(sizeof(Task));
		t->tid = head->task->tid;
		t->name = head->task->name;
		t->priority = head->task->priority;
		t->burst = head->task->burst;
		insert(copyL, t);
		head = head->next;
	}
	*copyL = (*copyL)->next;
	head = temp;
	while ((*copyL) != NULL){ // applying round robin on copied list
		if ((*copyL)->task->burst <= time_quantum){ // if burst is less than slice, simply run it
			run((*copyL)->task, (*copyL)->task->burst);
			tmpNode = *copyL;
			*copyL = (*copyL)->next;
			free(tmpNode->task);
		}
		else{// if burst is greater than slice, run for time slice, and insert again with decremented burst
			run((*copyL)->task, time_quantum);
			(*copyL)->task->burst -= time_quantum;
			insert(copyL, (*copyL)->task);
			tmpNode = *copyL;
			*copyL = (*copyL)->next;
			free(tmpNode);
		}
	}
}

// Function to find the waiting time for all
// processes
int findWaitingTime( struct node *L, int n, int wt[], int tat[]) {
   // Make a copy of burst times bt[] to store remaining
   // burst times.
   struct node *Position;
   Position = L;
   Position = Position->next;
   int rem_bt[n];
   int bt[n];
   int i = 0;

   while (Position != NULL){
   	rem_bt[i] = Position->task->burst;
   	bt[i++] = Position->task->burst;
   	Position = Position->next;
   	}

   int t = 0; // Current time
   // Keep traversing processes in round robin manner
   // until all of them are not done.
   while (1) {
      int done = 1;
      // Traverse all processes one by one repeatedly
      for (int i = 0 ; i < n; i++) {
         // If burst time of a process is greater than 0
         // then only need to process further
         if (rem_bt[i] > 0) {
            done = 0; // There is a pending process
            if (rem_bt[i] > time_quantum) {
               // Increase the value of t i.e. shows
               // how much time a process has been processed
               t += time_quantum;
               // Decrease the burst_time of current process
               // by quantum
               rem_bt[i] -= time_quantum;
            }
            // If burst time is smaller than or equal to
            // quantum. Last cycle for this process
            else {
               // Increase the value of t i.e. shows
               // how much time a process has been processed
               t = t + rem_bt[i];
               // Waiting time is current time minus time
               // used by this process
               tat[i] = t;
               wt[i] = t - bt[i];
               // As the process gets fully executed
               // make its remaining burst time = 0
               rem_bt[i] = 0;
            }
         }
      }
      // If all processes are done
      if (done == 1)
         break;
   }
   return 1;
}

//Function to calculate average time
void findavgTime(struct node *L, int n)
{
    int wt[n], tat[n], total_wt = 0, total_tat = 0;

    //Function to find waiting time of all processes
    findWaitingTime(L, n, wt, tat);
    puts("hello");
    //Display processes along with all details
    printf("Processes\tBurst time\tWaiting time\tTurn around time\n");
    L = L->next;
    // Calculate total waiting time and total turn
    // around time
    for (int  i=0; i<n; i++)
    {
        total_wt = total_wt + wt[i];
        total_tat = total_tat + tat[i];
        printf("\t\b%s",L->task->name);
        printf("\t\t%d", L->task->burst );
        printf("\t\t%d",wt[i]);
        printf("\t\t%d\n",tat[i]);
        if (L->next != NULL)
        L = L->next;
    }
    printf("Average waiting time = %.3f\n",total_wt / (float)n);
    printf("Average turn around time = %.3f\n ",total_tat / (float)n);
}
