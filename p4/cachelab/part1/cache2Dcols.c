////////////////////////////////////////////////////////////////////////////////
// Main File:        cache2Dcols
// This File:        cache2Dcols.c
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

int main() {
    // column wise looping
    for(int i = 0;i < 500;i++)
        for(int j = 0;j < 3000;j++)
            // iterates over the column first
            arr2D[j][i] = i + j;

    return 0;
}

