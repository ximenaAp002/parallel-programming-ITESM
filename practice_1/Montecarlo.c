#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

int npoints = 10000;
int circle_count = 0;
int rad = 1;
int side = 1;
double xcor;
double ycor;
double hipotenusa;

#define NUM_THREADS    5

void *pi(void *threadid){
    
    long tid;
    tid = (long)threadid;
    int j = 0;
    while ( j < npoints/NUM_THREADS){
        xcor = (double)(rand() % 1000)/1000 ;
        ycor = (double)(rand() % 1000)/1000 ;
        double cateto = xcor*xcor + ycor*ycor;
        hipotenusa = (cateto);
        if (hipotenusa <= 1){
            circle_count++;
        }
        j++;
        // printf("Xcor = %f, Ycor = %f\n", xcor, ycor);
        //printf("c = %d\n", circle_count);
        
    }
    double PI = 4.0*(double)circle_count/(double)npoints;
    printf("PI = %f\n", PI);
    pthread_exit(NULL);

}

int main(){

    pthread_t threads[NUM_THREADS];
    int num = NUM_THREADS;

    int rc;
    long t;
    printf("Numero de threads:  %d\n", num);
    for(t=0;t<NUM_THREADS;t++){
        // printf("In main: creating thread %ld\n", t);
        rc = pthread_create(&threads[t], NULL, pi, (void *)t);
        if (rc){
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }}

  pthread_exit(NULL);
}
