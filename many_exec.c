// Reina Eustache
// Assignment 1
// Due 03/01/2020 11:59 pm
// many_exec.c
// This file uses the fork() system call to create 4 immediate children from the 
// parent.  The program accepts 3 arguments from the command line, a file that will
// be used by the last 3 children, an integer for the number of lines printed by
// child 3, and a string utilized by child 4. Child 1 executes the cat /etc/passwd,
// Child 2 executes the wc -lw <file>, Child 3 executes head <number> <file>, Child 
// 4 executes grep -n <keyword>. All children utilize execv and command line 
// arguments from the user are validated to prevent the program from crashing. 

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>

int main(int argc, char **argv) {
    if (argc != 4) {  // validates that the user enetered the proper exact number of arguments
        printf("ERROR: This program accepts 3 arguments, you entered %d.\n", --argc);
        return -1;
    }

    if (access(argv[1], F_OK) == -1) { // ensures that the filename provided is valid
        printf("ERROR: Could not find the file '%s' you entered in argument 1.\n", argv[1]);
        return -1;
    }

    char *numLinesStr = argv[2];
    char *end;
    
    strtol(numLinesStr, &end, 10); // converts the string to an integer

    if (*end != '\0' || end == numLinesStr) { // checks if the string was properly converted
        printf("ERROR: You did not enter a number for argument 2, you entered %s.\n", argv[2]);
        return -1;
    }

    // argv[3] is already a string so no need to validate

    // arrays to pass to execv
    char *c1_argv[] = {"/bin/cat", "/etc/passwd", NULL}; // arr
    char *c2_argv[] = {"/usr/bin/wc", "-lw", argv[1], NULL};
    char *c3_argv[] = {"/usr/bin/head", "-n", argv[2], argv[1], NULL};
    char *c4_argv[] = {"/bin/grep", "-n", argv[3], argv[1], NULL};

    printf("\nParent process [%d] started!\n", getpid());

    if (fork() == 0) { // executed by 1st Child
        printf("\nResults of cat command:\n");
        execv(c1_argv[0], c1_argv);
    }
    else{ // executed by Parent
        wait(NULL); // waits for the 1st Child to finish

        if (fork() == 0){ // executed by 2nd Child
            printf("\nResults of wc command:\n");
            execv(c2_argv[0], c2_argv);
        }
        else { // executed by Parent
            wait(NULL); // waits for the 2nd Child to finish

            if (fork() == 0) { // executed by 3rd Child
                printf("\nResults of head command:\n");
                execv(c3_argv[0], c3_argv);
            }
            else { // executed by Parent
                wait(NULL); // waits for the 3rd Child to finish

                if (fork() == 0) { // executed by 4th Child
                    printf("\nResults of grep command:\n");
                    execv(c4_argv[0], c4_argv);
                }

                wait(NULL); // waits for the 4th Child to finish
                printf("\nParent is out! Bye!\n");

                return 0;
            }
        }
    }
}