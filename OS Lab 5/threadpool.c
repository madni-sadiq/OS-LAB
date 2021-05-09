/**
 * Implementation of thread pool.
 */

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include "threadpool.h"

#define NUMBER_OF_THREADS 3

// the work queue
Queue* Q;
void q_init(void) {
    Q = malloc(sizeof(Queue));
    Q -> Front = Q -> Rear = malloc(sizeof(node));
    Q -> Rear -> Next = NULL;
}

// the worker bee
pthread_t bee[NUMBER_OF_THREADS];
sem_t thread_sync, q_protect;

// returns 0 if successful or 1 otherwise,
int enqueue(task t) {
    node* tmp = malloc(sizeof(node));
    if(tmp==NULL){
        puts("Out of memory!!!");
        return 1;
    }
    Q->Rear->T = t;
    Q->Rear->Next = tmp;
    Q->Rear = tmp;
    tmp->Next = NULL;
    // sem_post(&q_protect);
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

    // execute the task
    sem_wait(&thread_sync);
    while(IsEmpty());
    // sem_wait(&q_protect);
    worktodo = dequeue();

    execute(worktodo.function, worktodo.data);
    sem_post(&thread_sync);
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
    int i = 0;
    q_init();
    sem_init(&thread_sync, 0, 1);
    // sem_init(&q_protect, 0, 0);

    for(i = 0; i < NUMBER_OF_THREADS; i++){
        pthread_create(&bee[i%3],NULL,worker,NULL);
    }
}

// shutdown the thread pool
void pool_shutdown(void) {
    int i;
    for(i = 0; i < NUMBER_OF_THREADS; i++)
        pthread_join(bee[i],NULL);
}
