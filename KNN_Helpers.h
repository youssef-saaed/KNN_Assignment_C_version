#ifndef __KNN_HELPERS_H__
#define __KNN_HELPERS_H__

// Include the standard library for memory allocation and other functions
#include <stdlib.h>
// Include the standard input/output library
#include <stdio.h>
// Include the math library for mathematical functions
#include <math.h>
// Include the time library for random number generation
#include <time.h>
// Include the header file that defines the data frame functions
#include "Dataframe.h"

// Define a macro that returns the maximum of two values
#define max(a, b) (a > b ? a : b)
// Define a macro that returns the minimum of two values
#define min(a, b) (a < b ? a : b)

// Declare a function that splits a data frame into training and testing data frames based on a given ratio and a label column
void split_samples(DataFrame* df, DataFrame **training_df, DataFrame **testing_df, double test_data_ratio, int labelcol);
// Declare a function that calculates the Euclidean distance between two points of the same length
double ecludian_dist(double *p1, double *p2, int len1); 
// Declare a function that performs the K-Nearest Neighbors algorithm on the training and testing data frames and prints the results
void KNN_training_res(DataFrame *training_df, DataFrame *testing_df, int k);

#endif