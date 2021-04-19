#include<stdio.h>
#include<time.h>
#include <pthread.h>
#include <stdlib.h>
#include<math.h>

double pointsinCircle;

pthread_mutex_t lock;
void *pointGen(void *Max_Points);/* the thread */

void main(int argc, char *argv[])
{
	int Max_Points = 1000;
 	/* the thread identifier */	
	pthread_attr_t attr; /* set of attributes for the thread */
	pthread_t tid[10];
	/* get the default attributes */
	pthread_attr_init(&attr);

	for(int i = 0; i < 10; i++){
		/* create the thread */	
		pthread_create(&tid[i],&attr,pointGen, &Max_Points);
		}
	/* now wait for the thread to exit */
	pthread_join(tid[0], NULL);
	pthread_join(tid[1], NULL);
	pthread_join(tid[2], NULL);
	pthread_join(tid[3], NULL);
	pthread_join(tid[4], NULL);
	pthread_join(tid[5], NULL);
	pthread_join(tid[6], NULL);
	pthread_join(tid[7], NULL);
	pthread_join(tid[8], NULL);
	pthread_join(tid[9], NULL);
	
	printf("pie = %.4f\n", 4 * (pointsinCircle/Max_Points));
	pthread_mutex_destroy(&lock);
}

/**
 * The thread will begin control in this function
 */
void *pointGen(void *Max_Points) 
{	
	pthread_mutex_lock(&lock);
	pointsinCircle = 0;
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
	pthread_mutex_unlock(&lock);
	pthread_exit(0);
	
} 
