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
	struct node **L = malloc(sizeof(struct node));
	*L = (struct node*)malloc(sizeof(struct node));
	head = head->next;
	struct node *temp;
	temp = head;
	printf("%p\n%p\n%p\n",head,temp,L);

	while (head != NULL){
		insert(L, head->task);
		head = head->next;
	}
	head = temp;
	while (head != NULL){
		if (head->task->burst <= time_quantum){
			run(head->task, head->task->burst);
			head = head->next;
		}
		else{
			run(head->task, time_quantum);
			head->task->burst -= time_quantum;
			insert(&head, head->task);
			head = head->next;		
		}
	}
}

void findWaitingTime(struct node *L, int n, int wt[]) 
{ 
    // waiting time for first process is 0 
    wt[0] = 0; 
    
    // calculating waiting time 
    for (int  i = 1; i < n ; i++ ) {
    	L = L->next;
        wt[i] =  L->task->burst + wt[i-1] ; 
        
        }
} 
    
// Function to calculate turn around time 
void findTurnAroundTime( struct node *L,  int n, int wt[], int tat[]) 
{ 
    // calculating turnaround time by adding 
    // bt[i] + wt[i] 
    for (int  i = 0; i < n ; i++) {
    	L = L->next;
        tat[i] = L->task->burst + wt[i];
        
        } 
} 
    
//Function to calculate average time 
void findavgTime( struct node *L, int n)  
{ 
    int wt[n], tat[n], total_wt = 0, total_tat = 0; 
    
    //Function to find waiting time of all processes 
    findWaitingTime(L, n, wt); 
    
    //Function to find turn around time for all processes 
    findTurnAroundTime(L, n, wt, tat); 
    
    //Display processes along with all details 
    printf("Processes\tBurst time\tWaiting time\tTurn around time\n"); 
    L = L->next;
    // Calculate total waiting time and total turn  
    // around time 
    for (int  i=0; i<n; i++) 
    { 
        total_wt = total_wt + wt[i]; 
        total_tat = total_tat + tat[i]; 
        printf("\t\bT%d",(i+1));
        printf("\t\t%d", L->task->burst );
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
