// Reina Eustache
// Assignment 2
// Due 03/25/2020 11:59 pm
// drawingStraws.c
// This file uses the pthread library to create 'n' number of threads, 'n'
// is provided at program invocation as a command line argument.  Error 
// checking is done so ensure validity of input.  Each spawned thread prints
// its corresponding thread number and a number randomly selected from the ones
// in the global array numbers but no two threads are allowed to pick the same 
// number and a final thread determines the loser thread who picked the smallest
// number.

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

// description for prototypes below
void *drawStraw(void *threadNum);
void *find_loser(void *param);

char numbers[10] = {2, 6, 32, 11, 12, 50, 55, 64, 4, 100};
char found[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // stores already picked random elements
char threadNums[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // stores thread numbers

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

    pthread_t tid[num + 1];
    srand(time(0));

    for(int i = 0; i < num; i++){ // create 'n' threads
        pthread_create(&tid[i], NULL, drawStraw, (void *)((long)i));
    }

    for(int i = 0; i < num; i++){ // ensure all spawned threads execute to completion
        pthread_join(tid[i], NULL);
    }

    pthread_create(&tid[num], NULL, find_loser, NULL); // created thread to determine loser
    pthread_join(tid[num], NULL); // ensure thread to determine loser finishes

    printf("All threads completed!\n");

    exit(0);
}

// void * drawStraw
// This function prints the thread number and a number randomly selected 
// from the global array 'numbers'. Ensures that no two threads randomly
// pick the same element by checking the value at that position in the 
// global 'found' array.
// threadNum - number of the thread executing the function
void *drawStraw(void *threadNum){
    long randNum = rand() % 10;

    while (found[randNum] == 1){ // generates a new random number until it finds a unique one
        randNum = rand() % 10;
    }

    found[randNum] = 1; // once it finds a unique one it sets that position as taken
    threadNums[randNum] = (long)threadNum; // stores that thread number who picked it for finding loser


    usleep(rand() % 1000000);
    printf("Thread %ld: drew %d\n", (long)threadNum, numbers[randNum]);

    pthread_exit(0);
}

// void * find_loser
// This function iterates through the 'found' array that contains the position of
// randomly picked numbers and finds which thread picked the smallest number
// from the array 'numbers'. It prints that thread as the loser.
void *find_loser(void *param){
    int smallest = 0;
    int threadNum = 0;
    int first = 0;

    for(int i = 0; i < 10; i++){ // iterate through 'found' array
        if (found[i] == 1){ // if a thread chose a number at that position
            if (first == 0){ // have not found first chosen number to set initial value
                smallest = numbers[i];
                threadNum = threadNums[i];
                first = 1;
            }
            if (numbers[i] < smallest){
                smallest = numbers[i];
                threadNum = threadNums[i];
            }
        } 
    }

    printf("Loser: Thread %d drew the smallest straw!\n", threadNum);
    pthread_exit(0);
}