// Reina Eustache
// Assignment 1
// Due 03/01/2020 11:59 pm
// fork_children.c
// This file uses the fork() system call to create 3 immediate children from the 
// parent: Child B, Child C and Child D. Child B then creates child E. Child B 
// calculates sum of all the numbers in an array, then waits for Child E  which 
// calculates the average of all the numbers to terminate and print before exiting. 
// ChildC counts the number of multiples of 5. ChildD counts the number of multiples
// of 7. Finally, the parent prints the result of its immediate children, B, C and D.

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#define SIZE 20

// Test data
int nums[SIZE] = {13, 5, 1, 13, 6, 6, 3, 7, 13, 3, 7, 2, 8, 15, 3, 12, 10, 7, 13, 10};
//int nums[SIZE] = {9, 6, 10, 5, 2, 14, 2, 5, 6, 2, 7, 8, 7, 14, 3, 10, 1, 2, 12, 3};
//int nums[SIZE] = {1, 11, 10, 2, 14, 2, 9, 1, 5, 5, 15, 10, 4, 12, 7, 7, 3, 9, 4, 12};

int main() {
    int i = 0; // variable to iterate through array in loop
    int statusE = 0; // stores return value of Child E

    double average = 0; // stores average from Child E
    int sum = 0; // stores sum from Child B
    int count5 = 0; // stores number of multiples of 5 from Child C
    int count7 = 0; // stores number of multiples of 7 from Child D

    // stores PID of immediate children returned by fork()
    pid_t childB = 0;
    pid_t childC = 0;
    pid_t childD = 0;

    printf("Parent process %d just started!\n", getpid());

    if ((childC = fork()) == 0) { //executed by Child C
        printf("[%d]: Forked process %d\n", getppid(), getpid());

        // counts the number of multiples of 5
        for (i = 0; i < SIZE; i++)
            if ((nums[i] % 5) == 0)
                count5++;

        exit(count5);
    }
    else { // executed by Parent
        if ((childD = fork()) == 0) { //executed by Child D
            printf("[%d]: Forked process %d\n", getppid(), getpid());

            // counts the number of multiples of 7
            for (i = 0; i < SIZE; i++)
                if ((nums[i] % 7) == 0)
                    count7++;

            exit(count7);
        }
        else { // executed by Parent
            if ((childB = fork()) == 0) { //executed by Child B
                printf("[%d]: Forked process %d\n", getppid(), getpid());

                if (fork() == 0) { //executed by Child E
                    printf("[%d]: Forked process %d\n", getppid(), getpid());

                    // sums all values in array for average
                    for (i = 0; i < SIZE; i++)
                        average += nums[i];

                    average = average / SIZE;
                    printf("[%d]: Average %0.2f\n", getpid(), average);
                    exit(100);
                }
                else { //executed by Child B
                    // sums all values in array
                    for (i = 0; i < SIZE; i++)
                        sum += nums[i];

                    wait(&statusE); // wait for Child E to terminate before exiting
                    exit(sum);
                }
            }
            else { // executed by Parent
                // to store PID of the immediate children returned by wait()
                pid_t wait1 = 0;
                pid_t wait2 = 0;
                pid_t wait3 = 0;

                // wait parameter - to store value returned by the chidren from calling exit()
                int val1 = 0;
                int val2 = 0; 
                int val3 = 0;

                // ensure that all children terminate before attempting to print
                wait1 = wait(&val1);
                wait2 = wait(&val2);
                wait3 = wait(&val3);

                // stores the exit status of each child
                val1 = WEXITSTATUS(val1);
                val2 = WEXITSTATUS(val2);
                val3 = WEXITSTATUS(val3);

                // finds which child returned val1 to know what it calculated
                if(wait1 == childB)
                    sum = val1;
                else if(wait1 == childC)
                    count5 = val1;
                else 
                    count7 = val1;
                
                // finds which child returned val2 to know what it calculated
                if(wait2 == childB)
                    sum = val2;
                else if(wait2 == childC)
                    count5 = val2;
                else
                    count7 = val2;

                // finds which child returned val3 to know what it calculated
                if(wait3 == childB)
                    sum = val3;
                else if(wait3 == childC)
                    count5 = val3;
                else 
                    count7 = val3;
                
                printf("[%d] Sum: %d\n", getpid(), sum);
                printf("[%d] Multiples of 5: %d\n", getpid(), count5);
                printf("[%d] Multiples of 7: %d\n", getpid(), count7);

                return 0;
            }
        }
    }
}
