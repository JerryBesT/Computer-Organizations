////////////////////////////////////////////////////////////////////////////////
// Main File:        cache2Drows
// This File:        cache2Drows.c
// Other Files:
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
// Persons:          Identify persons by name, relationship to you, and email.
//                   Describe in detail the the ideas and help they provided.
//
// Online sources:   avoid web searches to solve your problems, but if you do
//                   search, be sure to include Web URLs and description of
//                   of any information you find.
//////////////////////////// 80 columns wide ///////////////////////////////////

#include <stdio.h>

int arr2D[3000][500];

int main(int argc,char *argv[]) {
    // row wise looping
    for(int i = 0;i < 3000;i++)
        for(int j = 0;j < 500;j++)
            // iterates over the rows first
            arr2D[i][j] = i + j;

    return 0;
}
