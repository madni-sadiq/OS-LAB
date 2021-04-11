/**
 * list data structure containing the tasks in the system
 */

#include "task.h"

struct node {
    Task *task;
    struct node *next;
};

// insert and delete operations.
void insert(struct node **head, Task *task);
void delete(struct node **head, Task *task);
void traverse(struct node *head);


void add(char *name, int priority, int burst, struct node **L);
void schedule(struct node *head);
void findWaitingTime(struct node *L, int n, int wt[]);
