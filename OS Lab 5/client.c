/**
 * Example client program that uses thread pool.
 */

#include <stdio.h>
#include <unistd.h>
#include "threadpool.h"
#define size 10
struct data
{
    int a;
    int b;
};

void add(void *param) {
    struct data *temp;
    temp = (struct data*)param;

    printf("I add two values %d and %d result = %d\n",temp->a, temp->b, temp->a + temp->b);
}

void sum_arr(void* param) {
    int sum = 0, i;
    int* arr = (int*)param;

    printf("Sum of ");
    for(i = 0; i < size; i++){
        sum += (arr[i]);
        printf("%d ", (arr[i]));
    }
    printf("= %d\n", sum);
}

void do_nothing(void* nothing) {
    printf("Welcome to empty void function\n");
    sleep(2);
    puts("Waited for 2 seconds...");
}

int main(void)
{
    // create some work to do
    struct data work;
    int array[size], i;
    work.a = 5;
    work.b = 10;

    for (i = 0; i < size; i++){
        array[i] = i;
    }

    // initialize the thread pool
    pool_init();
    // submit the work to the queue
    pool_submit(&do_nothing,NULL);
    pool_submit(&add,&work);
    pool_submit(&sum_arr,array);

    pool_shutdown();

    return 0;
}
