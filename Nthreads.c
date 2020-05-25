// Reina Eustache 
// Assignment 2
// Due 03/25/2020 11:59 pm
// Nthreads.c
// This file uses the pthread library to create 'n' number of threads, 'n'
// is provided at program invocation as a command line argument.  Error 
// checking is done so ensure validity of input.  Each spawned thread prints
// its corresponding thread number and a number randomly selected from the ones
// in the global array numbers.

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

// description for prototype below
void *drawNumber(void *threadNum);

char numbers[10] = {2, 6, 32, 11, 12, 50, 55, 64, 4, 100};

int main(int argc, char **argv) {
    if (argc != 2) {  // validates that the user enetered the proper exact number of arguments
        printf("ERROR: This program accepts 1 argument, you entered %d.\n", --argc);
        return -1;
    }

    char *numLinesStr = argv[1];
    char *end;
    
    int num = strtol(numLinesStr, &end, 10); // converts the string to an integer

    if (*end != '\0' || end == numLinesStr) { // checks if the string was properly converted
        printf("ERROR: You did not enter a number for argument 1, you entered %s.\n", argv[1]);
        return -1;
    }

    if (num > 10 || num < 1){ // ensures the n provided is in the valid range
        printf("ERROR: Please enter a number in the valid range (0 < x <= 10), you entered %d.\n", num);
        return -1;
    }

    pthread_t tid[num];
    srand(time(0));

    for(int i = 0; i < num; i++){ // create 'n' threads
        pthread_create(&tid[i], NULL, drawNumber, (void *)((long)i));
    }

    for(int i = 0; i < num; i++){ // ensure all spawned threads execute to completion
        pthread_join(tid[i], NULL);
    }

    printf("All threads completed!\n");

    exit(0);
}

// void * drawNumber
// This function prints the thread number and a number randomly selected 
// from the global array 'numbers'.
// threadNum - number of the thread executing the function
void *drawNumber(void *threadNum){
    long randNum = rand() % 10;

    usleep(rand() % 1000000);
    printf("Thread %ld: drew %d\n", (long)threadNum, numbers[randNum]);

    pthread_exit(0);
}