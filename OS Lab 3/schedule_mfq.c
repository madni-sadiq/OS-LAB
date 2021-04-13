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
	struct node** theLists;
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
	
