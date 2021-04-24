/* 
To Compile 
gcc MonteCarlo.c -lpthread -lm

To run
./a.out

*/


#include<stdio.h>
#include<time.h>
#include <pthread.h>
#include <stdlib.h>
#include<math.h>

double pointsinCircle = 0;
void *pointGen(void *Max_Points);/* the thread */

void main(int argc, char *argv[]){
int Max_Points = 1000;
pthread_t tid; /* the thread identifier */
pthread_attr_t attr; /* set of attributes for the thread */
	
/* get the default attributes */
pthread_attr_init(&attr);

/* create the thread */
pthread_create(&tid,&attr,pointGen, &Max_Points);

/* now wait for the thread to exit */
pthread_join(tid,NULL);	

printf("pie = %.4f\n", 4 * (pointsinCircle/Max_Points)); 
}

/**
 * The thread will begin control in this function
 */
void *pointGen(void *Max_Points) 
{
	int n = *(int*) Max_Points;
	float x[n];
	float y[n];
	srand(time(0));
	for (int i = 0; i < n; i++)
	{
		x[i] = (double)rand() / (double)RAND_MAX;
		y[i] = (double)rand() / (double)RAND_MAX;
	}
	for (int i = 0; i < n; i++){
		if (sqrt(pow(x[i],2) + pow(y[i],2)) <= 1 ){
			pointsinCircle++;
		}	
	}
	pthread_exit(0);
} 
