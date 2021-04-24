/* 
To Compile 
gcc Fib7_15.c -lpthread

To run
./a.out

*/


#include<stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include<unistd.h>
int *fibser;
int done = 0; 

void *fibGen(void *Max_Points);/* the thread */

void main(int argc, char *argv[]){
int Max_Points;
printf("Enter the number of fibonacci numbers you want: ");
scanf("%d", &Max_Points);
fibser= (int*)malloc(sizeof(int)*Max_Points);
 /* the thread identifier */
pthread_attr_t attr; /* set of attributes for the thread */
pthread_t tid[Max_Points];	
/* get the default attributes */
pthread_attr_init(&attr);

for (int i=0; i<Max_Points; i++){
pthread_create(&tid[i],&attr,fibGen, &Max_Points);


}
while(1){
if (done){
for (int i=0; i<Max_Points; i++){
printf ("%d,", fibser[i]);
}

break;
}
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
	fibser[i]=i;
	}
	for (int i=2; i<n; i++){
	fibser[i] = fibser[i-1] + fibser[i-2];
	}
	done = 1;
	pthread_exit(0);
} 
