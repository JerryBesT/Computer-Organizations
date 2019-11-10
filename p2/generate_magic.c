////////////////////////////////////////////////////////////////////////////////
// Main File:        generate_magic.c
// This File:        generate_magic.c
// Other Files:      verify_hetero.c, hetero-3.txt
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

// Structure representing Square
// size: dimension(number of rows/columns) of the square
// array: 2D array of integers
typedef struct _Square {
	int size;
	int **array;
} Square;

int get_square_size();
Square * generate_magic(int size);
void write_to_file(char *filename, Square * square);

int main(int argc, char *argv[])                 
{	
	char *filename = argv[1];
	// Check input arguments to get filename
	if(argc != 2)
	{
		printf("Usage: ./generate_magic <filename>\n");
		exit(1);
	}
	// Get size from user
	int size = get_square_size();
	// Generate the magic square
	Square *magicSquare = generate_magic(size);
	// Write the square to the output file
	write_to_file(filename, magicSquare);
	return 0;
}

/* get_square_size prompts the user for the magic square size
 * checks if it is an odd number >= 3 and returns the number
 */
int get_square_size()            
{
	// prompt for input
	printf("Enter size of magic square, must be odd\n");
	
	// store the input
	int size = 0;

	// get the input from console
	scanf("%d", &size); //remark

	// check for proper boundary
	if((size % 2) != 1 || size < 3)
	{
		printf("Size must be an odd number >= 3.\n");
		exit(1);
	}
	
	// return input
	return size;
}

/* generate_magic constructs a magic square of size n
 * using the Siamese algorithm and returns the Square struct
 */
Square * generate_magic(int n)           
{
	// construct a new square
	Square *sq;
	
	// if not able to allocate memory
	if((sq = malloc(sizeof(Square))) == NULL)
	{
		printf("Not able to allocate memory on heap");
		exit(0);
	}
	
	// allocate memory for 2D array
	sq->array = malloc(sizeof(int*)*n);
	for(int a = 0;a < n;a++)
		*(sq->array + a) = malloc(sizeof(int*) * n);
	// in order to check for existing index
	for(int b = 0;b < n;b++)
		for(int c = 0;c < n;c++)
			*(*(sq->array + b) + c) = 0;

	// declare variables for rows, columns, indexes
	int row = 0;
	int column = n / 2;
	int index = 2;

	// assign indexes every time
	*(*(sq->array + row) + column) = 1;
	while(index <= n*n)
	{
		// set temp vars to prevent re-visiting
		int tempRow = row - 1;
		int tempCol = column + 1;

		// check for boundary
		if(tempRow < 0)
			tempRow = n - 1;
		if(tempCol > n - 1)
			tempCol = 0;

		// check if it already exists
		if(*(*(sq->array + tempRow) + tempCol) != 0)
		{
			// use the original index
			*(*(sq->array + row + 1) + column) = index;
			// prevent crossing boundary
			row = (row + 1)	% n;
		}
		else
		{	
			// store index
			*(*(sq->array + tempRow) + tempCol) = index;
			// upate the row and column
			row = tempRow;
			column = tempCol;
		}
		index++;
	}
	// Construct the square needed
	sq->size = n;
	
	return sq;
}

/* write_to_file opens up a new file(or overwrites the existing file)
 * and writes out the square in the format expected by verify_hetero.c
 */
void write_to_file(char *filename, Square * square)              
{
	// Write the square to file
	FILE *file = fopen(filename, "w");

	// if file is not existing
	if(file == NULL)
	{
		printf("Cannot open file for writing.\n");
		exit(1);
	}

	// get the pointers from the square constructed
	int **newArray = square->array;
	int newSize = square->size;

	// write the size
	fprintf(file, "%d", newSize);

	// write the square
	for(int i = 0;i < newSize;i++)
	{
		//new line
		fprintf(file,"\n%d", *(*(newArray + i)));	
		for(int j = 1;j < newSize;j++)
		{
			fprintf(file,",%d",*(*(newArray + i) + j));						
		}
	}
	fprintf(file,"\n");
	// close file
	fclose(file);	

	// memory freeing
	for(int i = 0;i < newSize;i++)
		free(*(square->array + i));
	free(square);

	// set to null for safety
	square = NULL;
}
