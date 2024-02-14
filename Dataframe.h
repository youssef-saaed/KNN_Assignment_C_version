#ifndef __DATAFRAME_H__
#define __DATAFRAME_H__

// Include the standard input/output library
#include <stdio.h>
// Include the string manipulation library
#include <string.h>
// Include the standard library for memory allocation and other functions
#include <stdlib.h>
// Include the math library for mathematical functions
#include <math.h>
// Include the header file that defines the data frame structures
#include "DataframeStructs.h"

// Declare a function that creates a new cell and returns a pointer to it
Cell* new_cell();
// Declare a function that copies a cell and returns a pointer to the copy
Cell* copy_cell(Cell *c);
// Declare a function that parses a string and returns a pointer to a cell with the corresponding data and type
Cell* parse_cell(char *str);
// Declare a function that creates a new data frame with the given number of rows and columns and returns a pointer to it
DataFrame* new_dataframe(int nrows, int ncols);
// Declare a function that reads a CSV file and returns a pointer to a data frame with the data from the file
DataFrame* read_csv(const char *filename);
// Declare a function that splits a line by a delimiter and returns the number of tokens and a pointer to an array of strings
int split(char *line, char delimiter, char ***strlist);
// Declare a function that copies a data frame and returns a pointer to the copy
DataFrame* copy_dataframe(DataFrame* df);
// Declare a function that calculates the mean of a column in a data frame and returns the result as a double
double mean(DataFrame *df, int col);
// Declare a function that calculates the standard deviation of a column in a data frame and returns the result as a double
double stdev(DataFrame *df, int col);
// Declare a function that standardizes a data frame by subtracting the mean and dividing by the standard deviation for the given columns and returns a pointer to the standardized data frame
DataFrame* standardize(DataFrame *df, char **cols, int cCols);
// Declare a function that prints a cell to the standard output
void print_cell(Cell *c);
// Declare a function that deletes a data frame and frees the memory allocated for it
void delete_dataframe(DataFrame **df);
// Declare a function that prints a data frame to the standard output
void print_dataframe(DataFrame *df);


#endif