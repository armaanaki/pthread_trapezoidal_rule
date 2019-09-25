#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "timer.h"

long thread_count, per_thread;
volatile double sum, h;
volatile long a, b, n;
pthread_mutex_t mutex;

void* trapezoidal_rule(void* rank);

void usage(char* prog_name);
void get_args(int argc, char* argv[]);
double function(double x);


int main(int argc, char* argv[]) {
    long thread;
    pthread_t* thread_handles;
    double start, finish;

    /* get args and set rng */
    get_args(argc, argv);
    per_thread = n/thread_count;

    thread_handles = (pthread_t*) malloc (thread_count*sizeof(pthread_t));
    pthread_mutex_init(&mutex, NULL);

    h = (b-a)/(double) n;
    sum = (function(a) + function(b))/2.0;

    GET_TIME(start);
    for (thread = 0; thread < thread_count; thread++) 
        pthread_create(&thread_handles[thread], NULL, trapezoidal_rule, (void*) thread);
    for (thread = 0; thread < thread_count; thread++)
        pthread_join(thread_handles[thread], NULL);

    sum = h * sum;
    GET_TIME(finish);

    printf("approximation is: %.15f\n", sum);
    printf("time = %e seconds\n", finish - start);

    pthread_mutex_destroy(&mutex);
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
    double x_i;
    long my_start, my_end;

    my_start = my_rank * per_thread;
    my_end = my_start + per_thread;

    for (my_start; my_start < my_end; my_start++) {
        x_i = a + my_start*h;
        local_sum += function(x_i);
    }

    pthread_mutex_lock(&mutex);
    sum += local_sum;
    pthread_mutex_unlock(&mutex);
    return NULL;
}

/*
 * Function: function
 * Purpose: return an f(x) given x
 * In args: x
 * Out args: f(x) or y
 */
double function(double x) {
    return x*x;
}

/*
 * Function:    get_args
 * Purpose:     get command line args
 * In args:     argc, argv
 * Globals out: thread_count
 */
void get_args(int argc, char* argv[]) {
    if (argc != 5) usage(argv[0]);
    thread_count = strtol(argv[1], NULL, 10);
    if (thread_count < 0) usage(argv[0]);
    n = strtol(argv[2], NULL, 10);
    if (n % thread_count != 0) usage(argv[0]);
    a = strtol(argv[3], NULL, 10);
    b = strtol(argv[4], NULL, 10);
    if (a > b) usage(argv[0]);
}

/*
 * Function:    usage
 * Purpose:     print usage if program run incorrectly
 * In args:     prog_name
 */
void usage(char* prog_name) {
    fprintf(stderr, "usage: %s <number of threads> <n> <a> <b>\n", prog_name);
    fprintf(stderr, "a is one side of integral, b is the other. n are the total subintervals\n");
    fprintf(stderr, "number of threads should be divisble by n\n");
    fprintf(stderr, "a < b\n");
    exit(1);
}
