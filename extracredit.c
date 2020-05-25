// Reina Eustache
// Assignment 3
// Due 05/10/2020 11:59 pm
// extracredit.c
// This file uses the pthread library to execute a total number of threads with 
// with only a certain number (max) of them running concurrently with the use
// of mutexes and condition variables. 

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <semaphore.h>

void *shopping(void *threadNum);

int totalNum = 0; // total number of customers currently inside the store
int maxCustomers = 0; // max customers that can be in the store at the same time

pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t numMax; // to signal when another customer can enter since one has left

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
    maxCustomers = num1;

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

    pthread_mutex_destroy(&m1);
    pthread_cond_destroy(&numMax);

    pthread_exit(NULL);
}

void *shopping(void *threadNum) {
    int num = (long)threadNum;

    pthread_mutex_lock(&m1); // acquire the lock
    while (totalNum == maxCustomers) { // incoming customers should wait if store is at maximum
        pthread_cond_wait(&numMax, &m1);
    }

    totalNum++;
    printf("Customer #%d: I'm in! (current-customers-in: %d)\n", num, totalNum);
    pthread_mutex_unlock(&m1); // release the lock

    usleep(rand() % 500000);

    pthread_mutex_lock(&m1); // acquire the lock
    totalNum--;
    printf("Customer #%d: left! (current-customers-in: %d)\n", num, totalNum);

    if (totalNum < maxCustomers) { // signal any waiting customers that there is now room to enter
        pthread_cond_signal(&numMax);
    }
    pthread_mutex_unlock(&m1); // release the lock

    pthread_exit(NULL);
}