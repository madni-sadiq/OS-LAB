/**
 * list data structure containing the tasks in the system
 */

#include "task.h"
#define MIN_PRIORITY 1
#define MAX_PRIORITY 10

struct node {
    Task *task;
    struct node *next;
};

// insert and delete operations.
void insert(struct node **head, Task *task);
void delete(struct node **head, Task *task);
void traverse(struct node *head);


// add a task to the list
void add(char *name, int priority, int burst, struct node **head);

// invoke the scheduler
void schedule(struct node *head);
void findavgTime( struct node *L, int n);
