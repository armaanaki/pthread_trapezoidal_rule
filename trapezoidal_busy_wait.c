#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

long thread_count;
volatile int flag;
volatile double sum;

void* trapezoidal_rule(void* rank);

void usage(char* prog_name);
void get_args(int argc, char* argv[]);


int main(int argc, char* argv[]) {
    long thread;
    pthread_t* thread_handles;
    double start, finish;

    /* get args and set rng */
    get_args(argc, argv);

    thread_handles = (pthread_t*) malloc (thread_count*sizeof(pthread_t));


    for (thread = 0; thread < thread_count; thread++) 
        pthread_create(&thread_handles[thread], NULL, trapezoidal_rule, (void*) thread);
    for (thread = 0; thread < thread_count; thread++)
        pthread_join(thread_handles[thread], NULL);

    printf("time = %e seconds\n", finish - start);
    free(thread_handles);
}

/*
 * Function: trapezoidal_rule 
 * Purpose: 
 * In args: 
 * Global in: thread_count
 * Global out: 
 */
void* trapezoidal_rule(void* rank) {
    long my_rank = (long) rank;
    double local_sum;

    /* CODE FOR TRAPEZOIDAL RULE GOES HERE*/

    while (flag != my_rank);
    sum += local_sum;
    flag = (flag+1) % thread_count;
    return NULL;
}

/*
 * Function:    get_args
 * Purpose:     get command line args
 * In args:     argc, argv
 * Globals out: thread_count
 */
void get_args(int argc, char* argv[]) {
    if (argc != 2) usage(argv[0]);
    thread_count = strtol(argv[1], NULL, 10);
    if (thread_count < 0) usage(argv[0]);
}

/*
 * Function:    usage
 * Purpose:     print usage if program run incorrectly
 * In args:     prog_name
 */
void usage(char* prog_name) {
    fprintf(stderr, "usage: %s <number of threads>\n", prog_name);
    fprintf(stderr, "number of threads \n");
    exit(1);
}
