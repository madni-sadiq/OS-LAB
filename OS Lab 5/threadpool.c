/**
 * Implementation of thread pool.
 */

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include "threadpool.h"

#define NUMBER_OF_THREADS 3
void execute(void (*somefunction)(void *p), void *p);

// Global Variables
Queue* Q;
pthread_t bee[NUMBER_OF_THREADS];
sem_t thread_sync, q_protect;
int shutdown_call = 0;

// initializing the Queue
void q_init(void) {
    Q = malloc(sizeof(Queue));
    Q -> Front = Q -> Rear = malloc(sizeof(node));
    Q -> Rear -> Next = NULL;
}

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
    sem_post(&q_protect);
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
    while(1){
        sem_wait(&thread_sync);
        sem_wait(&q_protect);
        if(shutdown_call) break;
        worktodo = dequeue();

        execute(worktodo.function, worktodo.data);
        sem_post(&thread_sync);
    }
    sem_post(&q_protect);
    sem_post(&thread_sync);
    pthread_exit(0);
}

/**
 * Executes the task provided to the thread pool
 */
void execute(void (*somefunction)(void *p), void *p) {
    (*somefunction)(p);
}

void free_mem(void){
    node* tmp;
    while(Q->Front->Next){
        tmp = Q -> Front;
        Q -> Front = Q -> Front -> Next;
        free(tmp);
    }
    free(Q->Front);
    free(Q);

    sem_destroy(&q_protect);
    sem_destroy(&thread_sync);
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
    sem_init(&q_protect, 0, 0);

    for(i = 0; i < NUMBER_OF_THREADS; i++){
        if(pthread_create(&bee[i],NULL,worker,NULL)!=0)
            puts("Cant create thread!!!");
            return;
    }
}

// shutdown the thread pool
void pool_shutdown(void) {
    int i, sem_val;
    shutdown_call = 1;
    if(sem_getvalue(&q_protect, &sem_val)==0 && sem_val <= 0)
        sem_post(&q_protect);

    for(i = 0; i < NUMBER_OF_THREADS; i++)
        pthread_join(bee[i],NULL);
    free_mem();
}