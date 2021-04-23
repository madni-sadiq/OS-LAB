#include<stdio.h>
#include <pthread.h>
#include <stdlib.h>

int *fibser;

void *fibGen(void *Max_Points);/* the thread */

void main(int argc, char *argv[]){
int Max_Points;
printf("Enter the number fibonacci numbers you want: ");
scanf("%d", &Max_Points);
fibser= (int*)malloc(sizeof(int)*Max_Points);
pthread_t tid; /* the thread identifier */
pthread_attr_t attr; /* set of attributes for the thread */
	
/* get the default attributes */
pthread_attr_init(&attr);

/* create the thread */
pthread_create(&tid,&attr,fibGen, &Max_Points);

/* now wait for the thread to exit */
pthread_join(tid,NULL);	
for (int i=0; i<Max_Points; i++){
printf ("%d,", fibser[i]);
}
printf("\b\n");
}

/**
 * The thread will begin control in this function
 */
void *fibGen(void *Max_Points) 
{
	int n = *(int*) Max_Points;
	for (int i=0; i<2; i++){
	//fibser[i]= *(int*)malloc(sizeof(int));
	fibser[i]=i;
	}
	for (int i=2; i<n; i++){
	//fibser[i] = *(int*)malloc(sizeof(int));
	fibser[i] = fibser[i-1] + fibser[i-2];
	}
	
	pthread_exit(0);
} 
