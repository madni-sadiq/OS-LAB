#define MIN_PRIORITY 1
#define MAX_PRIORITY 10
#define time_quantum1 10
#define time_quantum2 20
#define time_quantum3 30
#include "task.h"
#include "list.h"
#include "cpu.h"
#include<stdlib.h>
#include<stdio.h>
int tid = 0;
struct node** theLists;
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
    	int  i;

    /*  array of Lists of size=3
    */
    theLists = malloc(sizeof(struct node*) * 3);
    for(i = 0; i < 3; i++){
        theLists[i] = malloc(sizeof(struct node));
        theLists[i] -> next = NULL;
    }

	head = head->next;

	while (head != NULL){
		Task *t = malloc(sizeof(Task));
		t->tid = head->task->tid;
		t->name = head->task->name;
		t->priority = head->task->priority;
		t->burst = head->task->burst;
		insert(&theLists[0], t);
		head = head->next;

	}
	theLists[0] = theLists[0]->next;
	while (theLists[0] != NULL){
		if (theLists[0]->task->burst > time_quantum1){
			run(theLists[0]->task, time_quantum1);
		}
		else{
			run(theLists[0]->task, theLists[0]->task->burst);
		}
		if (theLists[0] ->task-> burst > time_quantum1){
			Task *t = malloc(sizeof(Task));
			t->tid = theLists[0]->task->tid;
			t->name = theLists[0]->task->name;
			t->priority = theLists[0]->task->priority;
			t->burst = theLists[0]->task->burst - time_quantum1;
			insert(&theLists[1], t);
		}
		theLists[0] = theLists[0]->next;
	}
	theLists[1] = theLists[1]->next;
	while (theLists[1] != NULL){
		if (theLists[1]->task->burst > time_quantum2){
			run(theLists[1]->task, time_quantum2);
		}
		else{
			run(theLists[1]->task, theLists[1]->task->burst);
		}
		if (theLists[1] ->task-> burst > time_quantum2){
			Task *t = malloc(sizeof(Task));
			t->tid = theLists[1]->task->tid;
			t->name = theLists[1]->task->name;
			t->priority = theLists[1]->task->priority;
			t->burst = theLists[1]->task->burst - time_quantum2;
			insert(&theLists[2], t);
		}
		theLists[1] = theLists[1]->next;
	}
	theLists[2] = theLists[2]->next;
	while (theLists[2] != NULL){
		if (theLists[2]->task->burst > time_quantum3){
			run(theLists[2]->task, time_quantum3);
		}
		else{
			run(theLists[2]->task, theLists[2]->task->burst);
		}
		if (theLists[2] ->task-> burst > time_quantum3){
			Task *t = malloc(sizeof(Task));
			t->tid = theLists[2]->task->tid;
			t->name = theLists[2]->task->name;
			t->priority = theLists[2]->task->priority;
			t->burst = theLists[2]->task->burst - time_quantum3;
			insert(&theLists[2], t);
		}
		theLists[2] = theLists[2]->next;
	}
}

void get_timing(int wt[], int bt[], int rem_bt[], int tat[], int quantum, int n){
    int i;
    static int t = 0;
    for(i = 0; i < n; i++){
        if(rem_bt[i] > quantum){
            t += quantum;
            rem_bt[i] -= quantum;
        }
        else if(rem_bt[i] == 0){
            continue;
        }
        else{
            t += rem_bt[i];
            rem_bt[i] = 0;
            tat[i] = t;
            wt[i] = t - bt[i];
        }
    }
}

void findavgTime(struct node *L, int n){
    int wt[n], tat[n], total_wt = 0, total_tat = 0;
    int rem_bt[n], bt[n], sum;
    // int wt0[n], wt1[n], wt2[n];
    int i;
    struct node* Position = L;
    for(i = 0; i < n; i++){
        rem_bt[i] = Position->next->task->burst;
        bt[i] = rem_bt[i];
        Position = Position -> next;
    }
    get_timing(wt, bt, rem_bt, tat, time_quantum1, n);
    get_timing(wt, bt, rem_bt, tat, time_quantum2, n);
    while(1){
        sum = 0;
        for(i = 0; i < n; i++) {
            sum += rem_bt[i];
        }
        if(sum)
            get_timing(wt, bt, rem_bt, tat, time_quantum3, n);
        else break;
    }

    printf("Processes\tBurst time\tWaiting time\tTurn around time\n");
    Position = L->next;
    // Calculate total waiting time and total turn
    // around time
    for (int  i=0; i<n; i++)
    {
        // wt[i] = wt0[i] + wt1[i] + wt2[i];
        total_wt = total_wt + wt[i];
        total_tat = total_tat + tat[i];
        printf("\t\b%s",Position->task->name);
        printf("\t\t%d", Position->task->burst );
        printf("\t\t%d",wt[i]);
        printf("\t\t%d\n",tat[i]);
        if (Position->next != NULL)
        Position = Position->next;
    }
    printf("Average waiting time = %.3f\n",total_wt / (float)n);
    printf("Average turn around time = %.3f\n ",total_tat / (float)n);


    return ;
}