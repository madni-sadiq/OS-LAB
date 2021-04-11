#define MIN_PRIORITY 1
#define MAX_PRIORITY 10
#include "task.h"
#include "list.h"
#include "cpu.h"
#include<stdlib.h>
#include<stdio.h>
int tid = 0;

void add(char *name, int priority, int burst, struct node **L){
	Task* t;
	struct node *newNode = malloc(sizeof(struct node));
	struct node *Position;

	t = malloc(sizeof(struct task));
	t->tid = tid+1;
	t->name = name;
	t->priority = priority;
	t->burst = burst;

	newNode -> task = t;
	newNode -> next = NULL;

	for (Position = *L; Position -> next != NULL; Position = Position -> next){
	    if(Position -> next -> task -> burst > burst){
	        newNode -> next = Position -> next;
	       // Position -> next = newNode;
	       break;
	    }
    }
    Position -> next = newNode;
}


// invoke the scheduler
void schedule(struct node *head){
	while (head->next != NULL){
		run(head->next->task, head->next->task->burst);
		head = head->next;
	}
}

void findWaitingTime(struct node *L, int n, int wt[])
{
    // waiting time for first process is 0
    wt[0] = 0;

    // calculating waiting time
    for (int  i = 1; i < n ; i++ ) {
        wt[i] =  L->next->task->burst + wt[i-1] ;
        L = L->next;
        }
}

// Function to calculate turn around time
void findTurnAroundTime( struct node *L,  int n, int wt[], int tat[])
{
    // calculating turnaround time by adding
    // bt[i] + wt[i]
    for (int  i = 0; i < n ; i++) {
        tat[i] = L->next->task->burst + wt[i];
        L = L->next;
        }
}

//Function to calculate average time
void findavgTime(struct node *L, int n)
{
    int wt[n], tat[n], total_wt = 0, total_tat = 0;

    //Function to find waiting time of all processes
    findWaitingTime(L, n, wt);

    //Function to find turn around time for all processes
    findTurnAroundTime(L, n, wt, tat);

    //Display processes along with all details
    printf("Processes\tBurst time\tWaiting time\tTurn around time\n");

    // Calculate total waiting time and total turn
    // around time
    for (int  i=0; i<n; i++)
    {
        total_wt = total_wt + wt[i];
        total_tat = total_tat + tat[i];
        printf("\t\bT%d",(i+1));
        printf("\t\t%d", L->next->task->burst );
        printf("\t\t%d",wt[i] );
        printf("\t\t%d\n",tat[i] );
        if (L->next != NULL)
        L = L->next;
    }
    int s=(float)total_wt / (float)n;
    int t=(float)total_tat / (float)n;
    printf("Average waiting time = %d",s);
    printf("\n");
    printf("Average turn around time = %d\n ",t);
}
