///////////////////////////////////////////////////////////////////////////////
// This File:        intdate.c
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

// global 3 second parameter
int second = 3;
// counting times of SIGUSR1
int counterUsr1 = 0;

void handler_sigalarm(int sig) {
    // initialize timer
    time_t timer = time(NULL);
    // if signal is alarm
    if (sig == SIGALRM) {
        printf("PID: %d | Current Time: %s", getpid(), ctime(&timer));
        // enable the looping of signals
        alarm(second);
    }
}

void handler_sigusr(int sig) {
    // if signal is user
    if (sig == SIGUSR1) {
        printf("SIGUSR1 caught!\n");
        // increment the times of SIGUSR1
        counterUsr1++;
    }
}

void handler_stop(int sig) {
    // if signal is terminate
    if (sig == SIGINT) {
        // the message printed after terminating
        printf("\nSIGINT received.\n");
        printf("SIGUSR1 was received %d times. Exiting now.\n", counterUsr1);
        exit(0);
    }
}

int main() {
    // SIGALRM
    printf("Pid and time will be printed every 3 seconds.\n");
    printf("Enter ^C to end the program.\n");
    // signal for alarm
    struct sigaction act;
    memset (&act, 0, sizeof(act));
    // set the handler function
    act.sa_handler = handler_sigalarm;
    act.sa_flags = 0;
    // catch the sinal 
    if (sigaction(SIGALRM, &act, NULL) != 0) {
        printf("Error for SIGALRM\n");
    }
    // set the time of the alarm
    alarm(second);

    // SIGUSR
    struct sigaction usr;
    memset (&usr, 0, sizeof(usr));
    // set the handler function for SIGUSR
    usr.sa_handler = handler_sigusr;
    // catch the signal
    if (sigaction(SIGUSR1, &usr, NULL) != 0) {
        printf("Error for SIGUSR1\n");
    }

    // SIGINT
    struct sigaction stop;
    memset (&stop, 0, sizeof(stop));
    // set the handler function
    stop.sa_handler = handler_stop;
    // catch the signal
    if (sigaction(SIGINT, &stop, NULL) != 0) {
        printf("Error for SIGINT\n");
    }

    // keep looping until terminated
    while (1) {
    }
    // end of program
    return 0;
}









