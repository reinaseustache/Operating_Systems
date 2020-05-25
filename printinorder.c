// Reina Eustache 
// Assignment 3
// Due 05/10/2020 11:59 pm
// printinorder.c
// This file uses the pthread library to create 4 threads that print different 
// piece of the line "Hang in there! Summer is almost here!" before printing 
// "The End!" using semaphores.

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

#define NUM_THREADS 4

// These threads print different pieces of a line that need to be printed in order
void *threadA(void * null);
void *threadB(void * null);
void *threadC(void * null);
void *threadD(void * null);

sem_t s1;
sem_t s2;
sem_t s3;
sem_t s4;

int main() {
    srand(time(NULL));

    pthread_t tid[NUM_THREADS];

    // initialize semaphores to 0 to enforce order of execution
    sem_init(&s1, 0, 0);
    sem_init(&s2, 0, 0);
    sem_init(&s3, 0, 0);
    sem_init(&s4, 0, 0);

    printf("Program started\n");

    pthread_create(&tid[0], NULL, threadA, NULL);
    pthread_create(&tid[1], NULL, threadB, NULL);
    pthread_create(&tid[2], NULL, threadC, NULL);
    pthread_create(&tid[3], NULL, threadD, NULL);

    // wait for "here!\n", the last piece to be printed
    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
    pthread_join(tid[2], NULL);
    pthread_join(tid[3], NULL);

    printf("The End!\n");

    sem_destroy(&s1);
    sem_destroy(&s2);
    sem_destroy(&s3);
    sem_destroy(&s4);

    pthread_exit(NULL);
}

void *threadA(void * null){
    printf("Hang in ");

    usleep(rand() % 50000);

    sem_post(&s1); // "there! " can now be printed
    sem_wait(&s4); // wait for "almost " to be printed 

    printf("here!\n");

    pthread_exit(NULL);
}

void *threadB(void * null){
    sem_wait(&s3); // wait for "is " to be printed

    usleep(rand() % 50000);
    printf("almost ");
    
    sem_post(&s4); // "here!\n" can now be printed

    pthread_exit(NULL);
}

void *threadC(void * null){
    sem_wait(&s2); // wait for "Summer " to be printed

    usleep(rand() % 50000);
    printf("is ");

    sem_post(&s3); // "almost " can now be printed

    pthread_exit(NULL);
}

void *threadD(void * null){
    sem_wait(&s1); // wait for "Hang in " to be printed
    printf("there! ");

    usleep(rand() % 50000);

    printf("Summer ");
    sem_post(&s2); // "is " can now be printed

    pthread_exit(NULL);
}