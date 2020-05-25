// Reina Eustache 
// Assignment 3
// Due 05/10/2020 11:59 pm
// shopping.c
// This file uses the pthread library to execute a total number of threads with 
// with only a certain number (max) of them running concurrently with the use
// of semaphores.

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <semaphore.h>

void *shopping(void *threadNum);

int totalNum = 0;

sem_t s1; // counting semaphore
sem_t s2; // binary semaphore

int main(int argc, char **argv) {
    if (argc != 3) {  // validates that the user enetered the proper exact number of arguments
        printf("ERROR: This program accepts 2 arguments, you entered %d.\n", --argc);
        return -1;
    }

    char *numLinesStr = argv[1];
    char *end;
    
    int num1 = strtol(numLinesStr, &end, 10); // converts the string to an integer

    if (*end != '\0' || end == numLinesStr) { // checks if the string was properly converted
        printf("ERROR: You did not enter a number for argument 1, you entered %s.\n", argv[1]);
        return -1;
    }

    char *num2LinesStr = argv[2];
    char *end2;
    
    int num2 = strtol(num2LinesStr, &end2, 10); // converts the string to an integer

    if (*end2 != '\0' || end2 == num2LinesStr) { // checks if the string was properly converted
        printf("ERROR: You did not enter a number for argument 2, you entered %s.\n", argv[2]);
        return -1;
    }

    pthread_t tid[num2];
    sem_init(&s1, 0, num1);
    sem_init(&s2, 0, 1);

    printf("Store is open for business! Welcome everyone!!!\n");

    // spawn all customer threads
    for(int i = 0; i < num2; i++){
        pthread_create(&tid[i], NULL, shopping, (void *)((long)i));
    }

    // wait for all customers to leave
    for (long i = 0; i < num2; i++) {
        pthread_join(tid[i], NULL);
    }

    printf("Store closed for the day! We open again tomorrow at 9am!\n");

    sem_destroy(&s1);
    sem_destroy(&s2);

    pthread_exit(NULL);
}

void *shopping(void *threadNum) {
    int num = (long)threadNum;

    sem_wait(&s1); // check counting semaphore to see if max customers has been reached
    sem_wait(&s2); // acquire "mutual exclusion lock"

    totalNum++;
    printf("Customer #%d: I'm in! (current-customers-in: %d)\n", num, totalNum);
    sem_post(&s2); // release "mutual exclusion lock"

    usleep(rand() % 500000);

    sem_wait(&s2); // acquire "mutual exclusion lock"
    totalNum--;
    printf("Customer #%d: left! (current-customers-in: %d)\n", num, totalNum);
    sem_post(&s1); // increment semaphore to allow another customer in
    sem_post(&s2); // release "mutual exclusion lock"

    pthread_exit(NULL);
}