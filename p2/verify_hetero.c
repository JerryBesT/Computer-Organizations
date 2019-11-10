////////////////////////////////////////////////////////////////////////////////
// Main File:        verify_hetero.c
// This File:        verify_hetero.c
// Other Files:      generate_magic.c , hetero-3.txt
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
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure representing Square
// size: dimension(number of rows/columns) of the square
// array: 2D array of integers
typedef struct _Square {
	int size;
	int **array;
} Square;

Square * construct_square(char *filename);
int verify_hetero(Square * square);
void insertion_sort(int* arr, int size);

int main(int argc, char *argv[])                        
{
	// Check input arguments to get filename
	// if argument is none, then exit
	if(argc != 2)
	{
		printf("usage: ./verify_hetero.c <filename>\n");
		exit(1);
	}
	// get the file name from argument line
	char *filename = argv[1]; //danger

	// Construct square	
	// construct the square from calling construct_square funtion
	Square *newsquare = construct_square(filename);

	// Verify if it's a heterosquare and print true or false
	int result = verify_hetero(newsquare); // remark
	// if the result is 1 then return true, then else.
	if(result == 1)
		printf("%s", "true\n");
	else
		printf("%s", "false\n");
}

/* construct_square reads the input file to initialize a square struct
 * from the contents of the file and returns the square.
 * The format of the file is defined in the assignment specifications
 */
Square * construct_square(char *filename)                
{
	// Open and read the file
	FILE *file = fopen(filename, "r");
	// if file can not be opened ,then exit	
	if(file == NULL){
	    printf("Cannot open file for reading.\n");
	    exit(1);
	}
	// Read the first line to get the square size
	int filesize = 0;

	// get the size of the square
	fscanf(file, "%d\n", &filesize); //remark
	
	// Initialize a new Square struct of that size
	// allocate a memory for square 
	Square *sq = malloc(sizeof(Square));

	// if square can not be allocated 
	if(sq == NULL)
	{
		printf("%s", "Not able to allocate memory on the heap.\n");
		exit(1);
	}

	//assign a size to the square
	sq->size = filesize;

	// allocate memory for the 2d array
	sq->array = malloc(sizeof(int*) * filesize);

	// if the memory can not be allocated
	if(sq->array == NULL)
	{
		printf("%s", "Not able to allocate memory on the heap.\n");
		exit(1);
	}
	
	// allocate column memory
	for(int i = 0;i < filesize;i++)
		*(sq->array + i) = malloc(sizeof(int*) * filesize);

	// Read the rest of the file to fill up the square
	// recored the current row that is reading 
	int row = 0;

	// construct a buffer string to get the lines
	char buff[255];

	// use the coma to separate the line elements
	const char s[2] = ",";

	// for separation
	char *token;

	// it continues when row < file size
	while(row < filesize)
	{
		// refresh the column every time
		int column = 0;
		// get the line
		fgets(buff,255,file);
		// separate
		token = strtok(buff, s);	
		while(token != NULL)
		{
			// convert string to int
			int value = atoi(token);
			// store the value
			*(*(sq->array + row) + column) = value;
			// increment column reading
			column++;
			token = strtok(NULL, s);
		}
		// get to the next line
		row++;
	}
	// close file
	fclose(file);
	return sq;
}

/* verify_hetero verifies if the square is a heterosquare
 * 
 * returns 1(true) or 0(false)
 */
int verify_hetero(Square * square)               
{
	// Calculate sum of the following and store it in an array
	// all rows and cols
	// main diagonal
	// secondary diagonal
	
	// get the size
	int squaresize = square->size;

	// get the constructed array
	int **squarearray = square->array;

	// allocate memory for the rows
	int* rowtotal = malloc(sizeof(int) * squaresize);
	// rowtotal array, store all the values to an array
	for(int a = 0;a < squaresize;a++)
	{
		int sum = 0;
		for(int b = 0;b < squaresize;b++)
		{
			sum = sum + *(*(squarearray + a) + b);
		}
		*(rowtotal + a) = sum;
	}

	// columntotal array
	int *columntotal = malloc(sizeof(int) * squaresize);
	for(int c = 0;c < squaresize;c++)
	{
		int columnsum = 0;
		for(int d = 0;d < squaresize;d++)
		{
			columnsum = columnsum + *(*(squarearray + d) + c);
		}
		*(columntotal + c) = columnsum;
	}

	// main diagnoal total
	int mainDtotal = 0;
	// add all the diagnoals up
	int mainDsum = 0;
	for(int e = 0;e < squaresize;e++)
	{
		mainDsum = mainDsum + *(*(squarearray + e) + e);
	}
	mainDtotal = mainDsum;

	// second diagonal total
	int secondDtotal = 0;
	// add the second diagonals up
	int secondDsum = 0;
	for(int f = 0;f < squaresize;f++)
	{
		secondDsum = secondDsum + *(*(squarearray + f) + (squaresize - f - 1));
	}
	secondDtotal = secondDsum;

	// the overall result from all the totals of rows, columns, main diagonal, second diagonal
	int *resultArray = malloc(sizeof(int) * (squaresize * 2 + 2));
	for(int h = 0;h < squaresize;h++)
		*(resultArray + h) = *(rowtotal + h);
	int indexC = 0;
	for(int i = squaresize;i < squaresize * 2;i++)
	{
		*(resultArray + i) = *(columntotal + indexC);
		indexC++;
	}
	*(resultArray + (squaresize * 2)) = mainDtotal;
	*(resultArray + (squaresize * 2 + 1)) = secondDtotal;

	//free all the memories
	free(rowtotal);
	free(columntotal);
	for(int i=0;i < squaresize;i++)
		free(*(square->array + i));	
	free(square->array);

	// Pass the array to insertion_sort function
	insertion_sort(resultArray , squaresize * 2 + 2);

	// Check the sorted array for duplicates
	for(int j = 0;j < squaresize * 2 + 2;j++)
	{
		int check = *(resultArray + j);
		for(int k = 0;k < squaresize * 2 + 2;k++)
		{
			// check any duplicates
			if(*(resultArray + k) == check && j != k)
				return 0;
		}
	}
	
	// free the memories
	free(resultArray);
	return 1;
}

/* insertion_sort sorts the arr in ascending order
 *
 */
void insertion_sort(int* arr, int size)
{
    // Sort the arr
    for(int a = 0;a < size - 1;a++)
    {
	// get the next value
        int tempValue = *(arr + a + 1);
	// recored the max swaps
        int tempindex = a;

	// as long as the swaping value is bigger than the next value
        while( *(arr + tempindex) > tempValue && tempindex >= 0)
        {
	    // swap positions
            *(arr + tempindex + 1) = *(arr + tempindex);
            tempindex--;
        }
	// put the value in a proper position
        *(arr + tempindex + 1) = tempValue;
    }
}
