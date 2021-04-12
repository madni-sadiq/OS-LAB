/**
 * Driver.c
 *
 * Schedule is in the format
 *
 *  [name] [priority] [CPU burst]
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "schedulers.h"

#define SIZE    100

int main(int argc, char *argv[])
{
    FILE *in;
    char *temp;
    char task[SIZE];
	struct node **L = malloc(sizeof(struct node));
    char *name;
    int priority;
    int burst;
	int n = 0;

	(*L) = (struct node*)malloc(sizeof(struct node));

    (*L) -> next = NULL;
    in = fopen(argv[1],"r");

    while (fgets(task,SIZE,in) != NULL) {
        temp = strdup(task);
        name = strsep(&temp,",");
        priority = atoi(strsep(&temp,","));
        burst = atoi(strsep(&temp,","));

        // add the task to the scheduler's list of tasks
        add(name,priority,burst,L);
		n++;
        free(temp);
    }

    fclose(in);
    //traverse(*L);
    // invoke the scheduler
    schedule(*L);
    findavgTime(*L, n);

    return 0;
}
