#include "task.h"
#include "list.h"
#include "cpu.h"
#include<stdlib.h>
#include<stdio.h>
#define MIN_PRIORITY 1
#define MAX_PRIORITY 10
#define time_quantum 10

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

void insert2list(struct node* L, Task* t){
    struct node* tmpNode = malloc(sizeof(struct node));
    struct node *lastNode = L;
    tmpNode -> task = t;
    tmpNode -> next = NULL;

    //last node's next address will be NULL.
    while(lastNode->next)
        lastNode = lastNode->next;

    //add the newNode at the end of the linked list
    lastNode->next = tmpNode;
}
void split(struct node** theLists, struct node *head){
    struct node* Position;
    for(Position = head->next; Position; Position = Position -> next){
        insert2list(theLists[Position -> task -> priority % (MAX_PRIORITY+1)], Position -> task);
    }
}

void schedule_rr(struct node *head){
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
		Task *t = malloc(sizeof(struct node));
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
// 			free(tmpNode->task);
			free(tmpNode);
		}
	}
}


void schedule(struct node *head){
    struct node** theLists;
    struct node* L;
    int size, i;

    /*  array of Lists of size=11
        size is 1 greater because MAX_PRIORITY % MAX_PRIORITY = 0
        one greater size to store MAX_PRIORITY at max index
    */
    theLists = malloc(sizeof(struct node*) * (MAX_PRIORITY+1));
    for(i = 0; i < (MAX_PRIORITY+1); i++){
        theLists[i] = malloc(sizeof(struct node));
        theLists[i] -> next = NULL;
    }
    split(theLists, head);

    for(size = (MAX_PRIORITY); size > 0; size--){
        L = theLists[size];
        if(L->next){
            schedule_rr(L);
        }
    }
}