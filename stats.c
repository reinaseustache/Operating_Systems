// Reina Eustache 
// Assignment 2
// Due 03/25/2020 11:59 pm
// stats.c
// This file uses the pthread library to spawn two threads to fill two arrays
// with random numbers, two more to sum the numbers in each array and two more
// to compute the minimum of each array. The master thread also prints the average
// using the resuts of the summation threads. Error checking is done so ensure 
// validity of input.

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

// description for prototypes below
void *fill(void *arrSize);
void *sum(void *arrSize);
void *minimum(void *arrSize);

char* arr1; // array 1
char* arr2; // array 2
int *arg1; // stores address of one argument to decide which array to fill
           // especially if they are the same size

int main(int argc, char **argv) {
    if (argc != 3) {  // validates that the user enetered the proper exact number of arguments
        printf("ERROR: This program accepts 2 argument, you entered %d.\n", --argc);
        return -1;
    }

    char *numLinesStr = argv[1];
    char *end;
    int size1 = strtol(numLinesStr, &end, 10); // converts the string to an integer

    if (*end != '\0' || end == numLinesStr) { // checks if the string was properly converted
        printf("ERROR: You did not enter a number for argument 1, you entered %s.\n", argv[1]);
        return -1;
    }

    numLinesStr = argv[2];
    end = NULL;
    int size2 = strtol(numLinesStr, &end, 10); // converts the string to an integer

    if (*end != '\0' || end == numLinesStr) { // checks if the string was properly converted
        printf("ERROR: You did not enter a number for argument 2, you entered %s.\n", argv[1]);
        return -1;
    }

     if (size1 < 1 || size2 < 1){ // ensures the n provided is in the valid range
        printf("ERROR: Please enter a number in the valid range (0 < x), you entered %d and %d.\n", size1, size2);
        return -1;
    }

    pthread_t tid[4];
    void * returnVal = 0; // stores value returned to master thread by spawned threads

    arr1 = malloc(size1);
    arr2 = malloc(size2);
    
    srand(time(0));

    arg1 = &size1;
    pthread_create(&tid[0], NULL, fill, (void *) &size1);
    pthread_create(&tid[1], NULL, fill, (void *) &size2);

    // wait because no computations can be done without the arrays being filled
    pthread_join(tid[0], NULL); 
    pthread_join(tid[1], NULL);

    printf("\nMaster thread: Array 1 = { ");
    for (int i = 0; i < size1; i ++)
        printf("%d ", arr1[i]);
    printf("}\n");

    printf("Master thread: Array 2 = { ");
    for (int i = 0; i < size2; i ++)
        printf("%d ", arr2[i]);
    printf("}\n");

    // run last 4 threads concurrently
    pthread_create(&tid[0], NULL, sum, (void *) &size1);
    pthread_create(&tid[1], NULL, sum, (void *) &size2);
    pthread_create(&tid[2], NULL, minimum, &size1);
    pthread_create(&tid[3], NULL, minimum, &size2);

    pthread_join(tid[0], &returnVal);
    printf("\nMaster thread: summation of Array 1 is %ld\n", (long)returnVal);
    printf("Master thread: average of Array 1 is %0.2f\n\n", (double)((long)returnVal) / size1);

    pthread_join(tid[1], &returnVal);
    printf("Master thread: summation of Array 2 is %ld\n", (long)returnVal);
    printf("Master thread: average of Array 2 is %0.2f\n\n", (double)((long)returnVal) / size2) ;

    pthread_join(tid[2], &returnVal);
    printf("Master thread: minimum of Array 1 is %ld\n", (long)returnVal);

    pthread_join(tid[3], &returnVal);
    printf("Master thread: minimum of Array 2 is %ld\n", (long)returnVal);

    // reclaim allocated memory
    free(arr1);
    free(arr2);
    
    exit(0);
}

// void * fill
// This function fills the two arrays with random numbers and
// prints as they are added
// arrSize - the size of the array to fill
void *fill(void *arrSize){
    if (arg1 == &(*(int *)arrSize)) //array 1
        for (int i = 0; i < * (int *)arrSize; i++){
            arr1[i] = rand() % 100 + 1;
            usleep(rand() % 1000000);
            printf("Thread 0: added %d to Array 1\n", arr1[i]);
        }
    else // array 2
        for (int i = 0; i < * (int *)arrSize; i++){
            arr2[i] = rand() % 100 + 1;
            usleep(rand() % 1000000);
            printf("Thread 1: added %d to Array 2\n", arr2[i]);
        }

    pthread_exit(0);
}

// void * sum
// This function sums the numbers in each array and returns the value
// to the master thread
// arrSize - the size of the array to fill
void *sum(void *arrSize){
    long sum = 0; 
    
    if (arg1 == &(*(int *)arrSize)) // array 1
        for (int i = 0; i < * (int *)arrSize; i++)
            sum += arr1[i];
    else // array 2
        for (int i = 0; i < * (int *)arrSize; i++)
            sum += arr2[i];
    
    return (void *) sum;
}

// void * minimum
// This function find the absolute minimumm of each array anf returns
// the value to the master thread
// arrSize - the size of the array to fill
void *minimum(void *arrSize){
    long smallest = 0;

    if (arg1 == &(*(int *)arrSize)){ // array 1
        smallest = arr1[0];

        for (int i = 1; i < * (int *)arrSize; i++)
            if (arr1[i] < smallest)
                smallest = arr1[i];
    } else { // array 2
        smallest = arr2[0];

        for (int i = 1; i < * (int *)arrSize; i++)
            if (arr2[i] < smallest)
                smallest = arr2[i];
    }

    return (void *) smallest;
}
