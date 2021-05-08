/**
 * Implementation of thread pool.
 */

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include "threadpool.h"

// #define QUEUE_SIZE 10
#define NUMBER_OF_THREADS 3

// #define TRUE 1

// the work queue
// task worktodo;
Queue* Q;
void q_init(void) {
    Q = malloc(sizeof(Queue));
    Q -> Front = Q -> Rear = malloc(sizeof(node));
    Q -> Rear -> Next = NULL;
}

// the worker bee
pthread_t bee[NUMBER_OF_THREADS];

// insert a task into the queue
// returns 0 if successful or 1 otherwise,
int enqueue(task t) {
    node* tmp = malloc(sizeof(node));

    Q->Rear->T = t;
	Q->Rear->Next = tmp;
	Q->Rear = tmp;
	tmp->Next = NULL;
    return 0;
}

// remove a task from the queue
task dequeue() {
    node* tmp = Q -> Front;
    task worktodo = tmp -> T;
    Q -> Front = Q -> Front -> Next;
    free(tmp);

    return worktodo;
}

int IsEmpty(void) {
    return (Q -> Front -> Next == NULL) ? 1 : 0;
}

// the worker thread in the thread pool
void *worker(void *param) {
    task worktodo;
    while(IsEmpty());   // wait for enqueue
    // execute the task
    worktodo = dequeue();
    execute(worktodo.function, worktodo.data);

    pthread_exit(0);
}

/**
 * Executes the task provided to the thread pool
 */
void execute(void (*somefunction)(void *p), void *p) {
    (*somefunction)(p);
}

/**
 * Submits work to the pool.
 */
int pool_submit(void (*somefunction)(void *p), void *p) {
    task worktodo;

    worktodo.function = somefunction;
    worktodo.data = p;

    return enqueue(worktodo);
}

// initialize the thread pool
void pool_init(void) {
    int i;
    q_init();
    for(i = 0; i < NUMBER_OF_THREADS; i++)
        pthread_create(&bee[i],NULL,worker,NULL);
}

// shutdown the thread pool
void pool_shutdown(void) {
    int i;
    for(i = 0; i < NUMBER_OF_THREADS; i++)
        pthread_join(bee[i],NULL);
}
