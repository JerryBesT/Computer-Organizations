///////////////////////////////////////////////////////////////////////////////
// This File:        division.c
// Other Files:      intdate.c, sendsig.c
// Semester:         CS 354 Spring 2018
//
// Author:           Zhenyu Zou
// Email:            zzou24@wisc.edu
// CS Login:         zzou
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:          Independent
//                   Identify persons by name, relationship to you, and email.
//                   Describe in detail the the ideas and help they provided.
//
// Online sources:   avoid web searches to solve your problems, but if you do
//                   search, be sure to include Web URLs and description of
//                   of any information you find.
//////////////////////////// 80 columns wide ///////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <signal.h>

// set up the counter for both function
int counter = 0;

// the handler for division error
void handler_sigdiv(int sig) {
    // message printed
    printf("Error: a division by 0 operation was attempted.\n");
    printf("Total number of operations completed successfully: %d\n", counter);
    printf("The program will be terminated.\n");
    exit(0);
}

// the handler for terminating
void handler_sigstop(int sig) {
    // message printed
    printf("\nTotal number of operations completed successfully: %d\n", 
            counter);
    printf("The program will be terminated.\n");
    exit(0);
} 
int main(int argc, char* argv[]) {
    // set up the action for division signal
    struct sigaction div;
    memset (&div, 0, sizeof(div));
    // set up the handler
    div.sa_handler = handler_sigdiv;
    // catch the signal
    if (sigaction(SIGFPE, &div, NULL) != 0) {
        printf("Error for SIGFPE\n");
    }

    // set up the action for division signal
    struct sigaction stop;
    memset (&stop, 0, sizeof(stop));
    // set up the handler
    stop.sa_handler = handler_sigstop;
    // catch the signal
    if (sigaction(SIGINT, &stop, NULL) != 0) {
        printf("Error for SIGINT\n");
    }

    // keep looping
    while (1) {
        // every char array buffer can only store one input from stdin
        char buf1[100];
        printf("Enter first integer: ");
        fgets(buf1, 100, stdin);
        // first input number from stdin
        int input1 = atoi(buf1);
        printf("Enter second integer: ");
        char buf2[100];
        fgets(buf2, 100, stdin);
        // second input number from stdin
        int input2 = atoi(buf2);
       
        // the result from dividing without remainder 
        int result = input1 / input2;
        // the remainder from dividing
        int remainder = input1 % input2;
        // message printed for dividing result
        printf("%d / %d is %d with a remainder of %d\n", 
                    input1, input2, result, remainder);
        // increment the successful dividing
        counter++; 
    } 
    return 0;   
}






