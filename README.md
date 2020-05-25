# Operating_Systems

Projects 

### Project 1
* fork_children.c
This file uses the fork() system call to create 3 immediate children from the parent: Child B, Child C and Child D. Child B then creates child E. Child B calculates sum of all the numbers in an array, then waits for Child E which calculates the average of all the numbers to terminate and print before exiting. ChildC counts the number of multiples of 5. ChildD counts the number of multiples of 7. Finally, the parent prints the result of its immediate children, B, C and D.

* many_exec.c
This file uses the fork() system call to create 4 immediate children from the parent.  The program accepts 3 arguments from the command line, a file that will be used by the last 3 children, an integer for the number of lines printed by child 3, and a string utilized by child 4. Child 1 executes the cat /etc/passwd, Child 2 executes the wc -lw <file>, Child 3 executes head <number> <file>, Child 4 executes grep -n <keyword>. All children utilize execv and command line arguments from the user are validated to prevent the program from crashing. 
  
### Project 2
* Nthreads.c
This file uses the pthread library to create 'n' number of threads, 'n' is provided at program invocation as a command line argument.  Error checking is done so ensure validity of input.  Each spawned thread prints its corresponding thread number and a number randomly selected from the ones in the global array numbers but no two threads are allowed to pick the same number and a final thread determines the loser thread who picked the smallest number.

* drawingStraws.c
This file uses the pthread library to create 'n' number of threads, 'n' is provided at program invocation as a command line argument.  Error checking is done so ensure validity of input.  Each spawned thread prints its corresponding thread number and a number randomly selected from the ones in the global array numbers.

* stats.c
This file uses the pthread library to spawn two threads to fill two arrays with random numbers, two more to sum the numbers in each array and two more to compute the minimum of each array. The master thread also prints the average using the resuts of the summation threads. Error checking is done so ensure validity of input.

### Project 3
* printinorder.c
This file uses the pthread library to create 4 threads that print different piece of the line "Hang in there! Summer is almost here!" before printing "The End!" using semaphores.

* shopping.c
This file uses the pthread library to execute a total number of threads with only a certain number (max) of them running concurrently with the use of semaphores.

* extracredit.c
This file uses the pthread library to execute a total number of threads with only a certain number (max) of them running concurrently with the use of mutexes and condition variables. 
