///////////////////////////////////////////////////////////////////////////////
// This File:        sendsig.c
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

int main(int argc, char *argv[]) {
    // if the arguments are not correct
    if  (argc != 3)
    {
        printf("Usage: <signal type> <pid>\n");
        exit(0);
    }

    // specify what action it is
    char *sigMode = argv[1];
    // what pid number it is
    int pid = atoi(argv[2]);
    // if is -i then terminate
    if (strcmp(sigMode, "-i") == 0)
        kill(pid, SIGINT);
    // if is -u then send SIGUSR1
    if (strcmp(sigMode, "-u") == 0)
        kill(pid, SIGUSR1); 
    return 0;
}




